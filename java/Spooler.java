package EPPC;

import java.io.*;
import java.util.concurrent.*;
import java.util.*;
import java.sql.*;


public class Spooler {

	public String registry;
	public Properties properties;
	public Properties test_properties;
	
	public BlockingQueue<EppTask> tasks;
	public BlockingQueue<MyEppClient> connects;
	public BlockingQueue<MyEppClient> test_connects;
	
	public List<Worker> workers;
	
	public volatile int MAX_WORKERS = 0;
	public volatile int MAX_CONNECTS = 0;
	
	public volatile int MAX_TEST_CONNECTS = 0;
	public volatile int MAX_TEST_WORKERS = 0;
	
	public volatile int connect_count = 0;
	
	public volatile boolean stopped = false;
	
	public final Object lock = new Object();
	
	static String namePool[] = {"Misaka", "Saber", "Tosaka", "Haruhi", "Ami", "Saten", "Madoka", "Flei", "Sora"};
	static int nameIter = 0;
	
	public final void info(String msg) {
		Walker.logger.info("["+registry.toUpperCase()+"] " + msg);
	}
	
	public final void debug(String msg) {
		Walker.logger.debug("["+registry.toUpperCase()+"] " + msg);
	}
	
	public final void error(String msg) {
		Walker.logger.error("["+registry.toUpperCase()+"] " + msg);
	}
	
	public Spooler(String registry) throws Exception {
	
		this.tasks = new LinkedBlockingQueue<EppTask>();
		this.registry = registry;
	
		try {
	
			// Loading production properties
	        String tld_config_name = "./production_" + this.registry + ".config";
	        info("Loading tld settings for registry " + this.registry + " from file " + tld_config_name);
	        
	        Properties props_tld = new Properties();
	        FileInputStream fis = new FileInputStream(tld_config_name);
	        props_tld.load(fis);
	        fis.close();
	        
	        // Loading test properties
	        String test_tld_config_name = "./test_" + this.registry + ".test.config";
	        info("Loading test tld settings for registry " + this.registry + " from file " + test_tld_config_name);
	        
	        Properties test_props_tld = new Properties();
	        FileInputStream test_fis = new FileInputStream(test_tld_config_name);
	        test_props_tld.load(test_fis);
	        test_fis.close();

	        this.MAX_WORKERS = Integer.parseInt(props_tld.getProperty("registry.max_workers"));
	        this.MAX_CONNECTS = Integer.parseInt(props_tld.getProperty("registry.max_connects"));
	        
	        this.MAX_TEST_WORKERS = Integer.parseInt(test_props_tld.getProperty("registry.max_workers"));
	        this.MAX_TEST_CONNECTS = Integer.parseInt(test_props_tld.getProperty("registry.max_connects"));
	        
	        this.properties = props_tld;
	        this.test_properties = test_props_tld;
	        this.registry = registry;    
	                
		} catch (Exception e) {
			error("Spooler startup error: " + e.getMessage());
			if(e.getCause() != null)
				error("Cause: " + e.getCause().getMessage());
			throw new Exception("It is unrecoverable error");
		}
	}
	
	public void start() {
	
		this.workers = new ArrayList<Worker>();
		this.connects = new LinkedBlockingQueue<MyEppClient>();
		this.test_connects = new LinkedBlockingQueue<MyEppClient>();
	
        info("Starting " + MAX_WORKERS + " worker threads");
        for(int i = 0; i < MAX_WORKERS; i++) {
        	createWorker(i);
        }
        
        // @todo: test workers?

	}
	
	public void close() throws InterruptedException {
	
		info("Terminating workers");

		stopped = true;

		for(Worker tmpwrk : workers) {
			tmpwrk.cancel();
			// Ждем завершения треда, чтобы спокойно отключиться потом от реестра
			try {
				tmpwrk.join();
			} catch (InterruptedException ex) {
				continue;
			}
		}
		
		workers.clear();

		info("Terminating connections");
		
		Set<MyEppClient> clients = new HashSet<MyEppClient>();
		connects.drainTo(clients);
		for(MyEppClient tmpcl : clients) {
		
			try {
				info("Sending LOGOUT command");
				tmpcl.logout();
	
			} catch (Exception e) {
				error("Failed to logout from registry: " + e.getMessage());
			}
			
			try {
				info("Disconnecting from registry");
				tmpcl.disconnect();
				
			} catch (Exception e) {
				error("Failed to disconnect from registry: " + e.getMessage());
			}
		}
		
		connects.clear();
	}
	
	public void createWorker(Integer i) {
	
		Worker worker = new Worker(this.properties, this.test_properties, this);
		if(nameIter < namePool.length) {
			worker.setName(namePool[nameIter]);
			nameIter++;
		}
		workers.add(worker);
		worker.start();
	}
	
	public void rescheduleConnect(ConnectScheduleTask task, java.util.Date wakeup) {
		synchronized(Walker.scheduler.lock) {
			Walker.scheduler.schedule(task, wakeup);
		}
	}
	
	public void incConnectCount() {
		synchronized(lock) {
			this.connect_count++;
		}
	}
	
	public void decConnectCount() {
		synchronized(lock) {
			this.connect_count--;
		}
	}
	
	public Integer getConnectCount() {
		synchronized(lock) {
			return this.connect_count;
		}
	}

	public void reschedulePollTask(EppPollTask task, java.util.Date wakeup) {
		synchronized(Walker.scheduler.lock) {
			info("Rescheduled poll task");
			
			Walker.scheduler.schedule(task, wakeup);
		}	
	}
	
	public void rescheduleTask(EppTask task, java.util.Date wakeup) {
		synchronized(Walker.scheduler.lock) {
			info("Rescheduled task #" + task.id);
			EppScheduleTask new_task = new EppScheduleTask();
			new_task.id = task.id;
			new_task.uid = task.uid;
			new_task.registry = task.registry;
			new_task.command = task.command;
			new_task.args = task.args;
			new_task.stamp = task.stamp;
			new_task.wakeup = task.wakeup;
			new_task.parent_id = task.parent_id;
			new_task.status = task.status;
			new_task.keyarg = task.keyarg;
			new_task.msg = task.msg;
			new_task.testing = task.testing;
			new_task.protocol = task.protocol;
			new_task.retry_count = task.retry_count;
			new_task.retry_interval = task.retry_interval;
			new_task.domain = task.domain;		
			
			Walker.scheduler.schedule(new_task, task.wakeup);
		}
	}
	
    public void deleteTask(Integer task_id) {
		
		debug("Checking parent task #" + task_id);
		debug("Waiting free postgres connection from pool");
		Connection conn = null;
		try {
			conn = Walker.pool.pool.take();
		} catch (InterruptedException e) {
			error("Interrupted while waiting postgres conn");
			Thread.currentThread().interrupt();
			return;
		}
		
		debug("Got postgres connection");
		
		try {
			info("Deleting task #" + task_id);
			PreparedStatement ps = conn.prepareStatement("DELETE FROM epp_queue WHERE id=?");
			ps.setInt(1, task_id);
			ps.executeUpdate();
			ps.close();

			Walker.scheduler.reportFinish(task_id);

		} catch (SQLException q) {
			error("deleteTask Error: " + q.getMessage());
		}
		
		debug("Putting postgres connection back to pool");
		Walker.pool.pool.add(conn);
    }
    
    public void updateTask(String msg, Integer code, Integer task_id, Integer epp_response_code) {
		
		debug("Checking parent task #" + task_id);
		debug("Waiting free postgres connection from pool");
		Connection conn = null;
		try {
			conn = Walker.pool.pool.take();
		} catch (InterruptedException e) {
			error("Interrupted while waiting postgres conn");
			Thread.currentThread().interrupt();
			return;
		}
		debug("Got postgres connection");
    		
		try {
			info("Updating task #" + task_id + ". Setting new status = " + code);	
			PreparedStatement ps = conn.prepareStatement("UPDATE epp_queue SET msg=?, status=?, epp_response_code=? WHERE id=?");
			ps.setString(1, msg == null ? "" : msg);
			ps.setInt(2, code);
			ps.setInt(3, epp_response_code);
			ps.setInt(4, task_id);
			ps.executeUpdate();
			ps.close();
		} catch (SQLException q) {
			error("updateTask Error: " + q.getMessage());
		}
		
		debug("Putting postgres connection back to pool");
		Walker.pool.pool.add(conn);		  
    }
    

}