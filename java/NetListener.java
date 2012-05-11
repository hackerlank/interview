package EPPC;

import java.io.*;
import java.nio.channels.*;
import java.nio.channels.spi.*;
import java.util.*;
import java.net.*;
import java.sql.*;
import java.lang.reflect.*;
import java.lang.management.RuntimeMXBean;
import java.lang.management.ManagementFactory;

public class NetListener extends Thread {
	
	String host;
	Integer port;
	
	final String availableCommands[] = {"status", "task", "last"};

	public NetListener(String host, Integer port) {
		setName("NetListener");
		setDaemon(true);
		this.host = host;
		this.port = port;
	}
	
	public String last(String in_count) throws SQLException {
		
		int count = -1;
		
		try {
			count = Integer.parseInt(in_count);
		} catch (Exception e) {
			Walker.logger.error("Invalid count: " + in_count);
			return ("Invalid count: " + in_count);
		}
		
		Connection pool_conn = null;
		
		try {
			Walker.logger.debug("Waiting for postgres connect");
			pool_conn = Walker.pool.pool.take();
		} catch (InterruptedException e) {
			Walker.logger.error("Interrupted while waiting for postgres connect");
			interrupt();
		}
		
		if(count > 25)
			count = 25;
		
		Walker.logger.debug("Getting info about tasks from DB");
		Statement st = pool_conn.createStatement();
		ResultSet rs = st.executeQuery("SELECT id FROM public.epp_queue_log ORDER BY sys_id DESC LIMIT " + count);

		String result = "";
		
		while(rs.next()) {
			String id = rs.getString("id");
			
			result += task(id);
		}
		
		return result;
	}
	
	public String task(String in_id) throws SQLException {
	
		int id = -1;
		
		try {
			id = Integer.parseInt(in_id);
		} catch (Exception e) {
			Walker.logger.error("Invalid id: " + in_id);
			return ("Invalid id: " + in_id);
		}
	
		Connection pool_conn = null;
		
		try {
			Walker.logger.debug("Waiting for postgres connect");
			pool_conn = Walker.pool.pool.take();
		} catch (InterruptedException e) {
			Walker.logger.error("Interrupted while waiting for postgres connect");
			interrupt();
		}
		
		Walker.logger.debug("Getting info about task from DB");
		Statement st = pool_conn.createStatement();
		ResultSet rs = st.executeQuery("SELECT * FROM public.epp_queue_log WHERE id = " + id);
		EppTask old_task = new EppTask();
		
		Boolean task_found = false;
		
		while(rs.next()) {
			task_found = true;
			old_task.id = rs.getInt("id");
			old_task.uid = rs.getInt("uid");
			old_task.registry = rs.getString("registry");
			old_task.command = rs.getString("command");
			old_task.args = new epp_type((String[]) rs.getArray("args").getArray());
			Calendar cal = Calendar.getInstance();
			cal.setTimeInMillis(rs.getTimestamp("stamp").getTime());
			old_task.stamp = cal.getTime();
			cal.setTimeInMillis(rs.getTimestamp("wakeup").getTime());
			old_task.wakeup = cal.getTime();
			old_task.parent_id = rs.getInt("parent_id");
			old_task.status = rs.getInt("status");
			old_task.keyarg = rs.getString("keyarg");
			old_task.msg = rs.getString("msg");
			old_task.testing = rs.getBoolean("testing");
			old_task.protocol = rs.getString("protocol");
			old_task.retry_count = rs.getInt("retry_count");
			old_task.retry_interval = rs.getInt("retry_interval");
		}
		
		rs.close();
		st.close();
		Walker.pool.pool.add(pool_conn);		
		
		String result = "";
		
		if(task_found) {
			Walker.logger.debug("Got info");
			result += "\n-------------------";
			result += "\nTask #" + id;
			result += "\n\tUID:\t\t\t" + old_task.uid;
			result += "\n\tRegistry:\t\t" + old_task.registry;
			result += "\n\tCommand:\t\t" + old_task.command;
			result += "\n\tArgs:\t\t\t" + old_task.args;
			result += "\n\tCreated:\t\t" + old_task.stamp;
			result += "\n\tWakeup:\t\t\t" + old_task.wakeup;
			result += "\n\tParent id:\t\t" + old_task.parent_id;
			result += "\n\tStatus:\t\t\t" + EppScheduleTask.statusToString(old_task.status);
			result += "\n\tKeyarg:\t\t\t" + old_task.keyarg;
			result += "\n\tMessage:\t\t" + old_task.msg;
			result += "\n\tTesting:\t\t" + old_task.testing;
			result += "\n\tProtocol:\t\t" + old_task.protocol;
			result += "\n\tRetry count:\t\t" + old_task.retry_count;
			result += "\n\tRetry interval:\t\t" + old_task.retry_interval;			
		}
		else
			result = "No such task";

		return result;
	}
	
	public String status() {
	
		RuntimeMXBean mx = ManagementFactory.getRuntimeMXBean();
		
		int uptime = (int)mx.getUptime();
		int seconds = (uptime/1000) % 60;
		int minutes = (uptime/(1000*60)) % 60;
		int hours = (uptime/(1000*60*60)) % 24;
		int days = (uptime/(1000*60*60) / 24);
		
		String str_uptime = days + "d " + hours + "h " + minutes + "m " + seconds + "s ";
		
		String result = "";
		
		result += "Status: \t\tRUNNING";
		result += "\nStart time: \t\t" + new java.util.Date(mx.getStartTime());
		result += "\nUptime: \t\t" + str_uptime;
		
		for(Spooler sp : Walker.spoolers) {
			result += "\n\nRegistry " + sp.registry.toUpperCase();
			result += "\n\tTasks in spooler: \t" + sp.tasks.size();
			result += "\n\tWorkers: \t" + sp.workers.size();
			
			for(Worker wrk : sp.workers) {
				result += "\n\t" + wrk.getName() + ":\tt. compl: " + wrk.total_tasks;
				result += "\ttotal t: " + wrk.total_time / 1000.0 + "s";
				result += "\tmin t: " + wrk.min_exec_time / 1000.0 + "s";
				result += "\tmax t: " + wrk.max_exec_time / 1000.0 + "s";
				result += "\t last task: " + (wrk.last_task != null ? ("[id="+wrk.last_task.id+"] "+wrk.last_task.command + " " + wrk.last_task.keyarg + ": " + EppScheduleTask.statusToString(wrk.last_status)) : "None");
			}
		}
		
		return result;
	}
	
	@Override
	public void run() {
	
		Selector acceptSelector = null;
		ServerSocketChannel ssc = null;
	
		try {
	
			Walker.logger.debug("Opening selector");
			acceptSelector = SelectorProvider.provider().openSelector();
			
			Walker.logger.debug("Opening non-blocking socket");
			ssc = ServerSocketChannel.open();
			ssc.configureBlocking(false);
			
			Walker.logger.debug("Getting inet addr");
			InetAddress h1 = InetAddress.getByName(host);
			InetSocketAddress isa = new InetSocketAddress(h1, port);
			
			Walker.logger.debug("Binding socket to "+host+":"+port);
			ssc.socket().bind(isa);
			
			SelectionKey acceptKey = ssc.register(acceptSelector, SelectionKey.OP_ACCEPT);
			
			Walker.logger.info("Listening on "+host+":"+port);
		
		} catch (Exception e) {
			Walker.logger.error("Failed to create non-blocking socket");
			if(e.getMessage() != null)
				Walker.logger.error(e.getMessage());
			if(e.getCause() != null)
				Walker.logger.error(e.getCause().getMessage());
				
			return;
		}
		
		int keysAdded = 0;
		
		while(!isInterrupted()) {
		
			try {
		
				while ((keysAdded = acceptSelector.select()) > 0) {
				
					try {
						// Someone is ready for I/O, get the ready keys
						Set readyKeys = acceptSelector.selectedKeys();
						Iterator i = readyKeys.iterator();
						
						while(i.hasNext()) {
							SelectionKey sk = (SelectionKey)i.next();
							i.remove();
							
							ServerSocketChannel nextReady = (ServerSocketChannel)sk.channel();
							
							Socket s = nextReady.accept().socket();
							
							PrintWriter out = new PrintWriter(s.getOutputStream(), true);
							BufferedReader in = new BufferedReader(new InputStreamReader(s.getInputStream()));
							
							String line = in.readLine().toLowerCase();
							
							if(line != null && line.length() > 0) {
							
								String[] command = line.split(" ", 2);
								String[] params = null;
								Class[] types = null;
								if(command.length > 1) {
									params = command[1].split(" ");
									types = new Class[params.length];
									for(int k = 0; k < params.length; k++)
										types[k] = params[k].getClass();
								}
							
								Boolean command_found = false;
							
								for(String iter : availableCommands) {
									if(command[0].equals(iter)) {
										Walker.logger.info("Command request: " + iter.toUpperCase());
										command_found = true;
										
										try {
										
											Class fc = this.getClass();
											
											Method m;
											Object result;
											
											if(params != null) {
												m = fc.getMethod(iter, types);
												result = m.invoke(this, (Object[])params);
											} else {
												m = fc.getMethod(iter);
												result = m.invoke(this);
											}

											result = (String)result;
											out.println(result);
											
										} catch (NoSuchMethodException e) {
											Walker.logger.error("No such method: " + iter);
										
										} catch (Exception e) {
											Walker.logger.error("Error executing method: " + e.getMessage());
											if(e.getCause() != null)
												Walker.logger.error("Cause: " + e.getCause().getMessage());
												
										}
										
									}
								}
								
								if(!command_found) {
									Walker.logger.info("Command not found: " + command[0]);
								}
							}
							
							out.close();
							in.close();
						}
						
					} catch (Exception e) {
						Walker.logger.error("Exception occured");
						if(e.getMessage() != null)
							Walker.logger.error(e.getMessage());
						if(e.getCause() != null)
							Walker.logger.error(e.getCause().getMessage());			
					}
				}
			} catch (IOException io) {
				Walker.logger.error("IO exception: " + io.getMessage());
				if(io.getCause() != null)
					Walker.logger.error("Cause: " + io.getCause().getMessage());
			}
		}
	}
}
