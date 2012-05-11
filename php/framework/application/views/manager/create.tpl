
<div class="form form-create" title="Create a new manager">

    <p class="validateTips"></p>
	<form method="POST">
	<fieldset>
        <div class="row">
		<label for="name">Name</label>
		<input type="text" name="name" id="name" class="text ui-widget-content ui-corner-all" />
        </div>
        <div class="row">
		<label for="login">Login</label>
		<input type="text" name="login" id="login" class="text ui-widget-content ui-corner-all" />
        </div>
        <div class="row">
		<label for="password">Password</label>
		<input type="password" name="password" id="password" value="" class="text ui-widget-content ui-corner-all" />
        </div>
        <div class="row">
		<label for="password">Password</label>
		<input type="password" name="password" id="password" value="" class="text ui-widget-content ui-corner-all" />
        </div>
        <div class="row">
		<label for="password">Password</label>
		<input type="password" name="password" id="password" value="" class="text ui-widget-content ui-corner-all" />
        </div>
        <div class="row">
		<label for="password">Password</label>
		<input type="password" name="password" id="password" value="" class="text ui-widget-content ui-corner-all" />
        </div>
        <div class="row">
		<label for="password">Password</label>
		<input type="password" name="password" id="password" value="" class="text ui-widget-content ui-corner-all" />
        </div>
        <div class="row">
		<label for="password">Password</label>
		<input type="password" name="password" id="password" value="" class="text ui-widget-content ui-corner-all" />
        </div>
        <div class="row">
            <input type="submit" name="submit" value="Login">
        </div>
	</fieldset>
	</form>

    {if count($errors) > 0}
        <div class="ui-widget">
        {foreach from=$errors item=error}
            <div class="ui-state-error ui-corner-all" style="margin-left: 10px; margin-right: 10px; width: 86%; font-size: 0.9em; padding-left: 1.1em;">
                <p><span class="ui-icon ui-icon-alert" style="float: left; margin-right: .3em; margin-top: .1em;"></span>
                    <strong>Alert:</strong> {$error}</p>
            </div>
        {/foreach}
        </div>
    {/if}

</div>