{if !(MintUser::getCurrentUser() instanceof MintAnonymousUser)}
<div id="userinfo">
    You are <b>{MintUser::getCurrentUser()->login}</b> | <a href="/account/logout">Logout</a>
</div>
{/if}