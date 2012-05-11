<ul>
{foreach from=$menu item=menu_item}
    {if $menu_item->parent_id != 0}
    <li class="child">
    {else}
    <li>
    {/if}
        {assign var=acl value=MintACLItem::getByPK($menu_item->acl_id)}
        <a href="/{$acl->controller_name}{if $acl->controller_method != 'index'}/{$acl->controller_method}{/if}" title="{$menu_item->title}">{$menu_item->title}</a>
    </li>
{/foreach}
</ul>