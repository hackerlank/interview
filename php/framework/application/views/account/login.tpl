{if $USE_OS}
<script type="text/javascript" src="/js/os/login.js"></script>
{/if}
<form class="form form-login" action="/account/login/" method="post" name="Авторизация">

    <div class="row">
        <label>Username</label>
          <input name="login" type="text" value="" id="login"/>
      </div>
       <div class="row">
       <label>Password</label>
          <input name="password"  type="password" value="" id="password" />
      </div>
        <div class="row">
            <input type="submit" value="Go" />
        </div>

    {if count($errors) > 0}
        <div class="ui-widget">
        {foreach from=$errors item=error}
            <div class="row ui-state-error ui-corner-all" style="margin-left: 10px; margin-right: 10px; width: 86%; font-size: 0.9em; padding-left: 1.1em; ">
                <p><span class="ui-icon ui-icon-alert" style="float: left; margin-right: .3em; margin-top: .1em;"></span>
                    <strong>Alert:</strong> {$error}</p>
            </div>
        {/foreach}
        </div>
    {/if}
  </form>