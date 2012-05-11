import tagging

from django.db import models
from datetime import datetime
from tagging.fields import TagField
from tagging.models import Tag
from django.contrib.auth.models import User
from django.db.models.signals import post_delete, post_save

class ContentType(models.Model):
    name = models.CharField(max_length=256)
    tag_name = models.CharField(max_length=128)
    created = models.DateTimeField(default=datetime.now)
    description = models.TextField()

class PastaUserManager(models.Manager):
    def get_by_natural_key(self, username):
        return self.get(username=username)

class PastaUser(models.Model):
    objects = PastaUserManager()

    def natural_key(self):
        return (self.user.username)

    user = models.OneToOneField(User)

    def __str__(self):
        return '%s' % self.user.username

    class Meta:
        unique_together = (('user',),)

def create_pasta_user(sender, instance, created, **kwargs):
    if created:
        pastauser, created = PastaUser.objects.get_or_create(user=instance)

post_save.connect(create_pasta_user, sender=User)


class Pasta(models.Model):
    author = models.ForeignKey(PastaUser, null=True, blank=True)
    date = models.DateTimeField(default=datetime.now)
    text = models.TextField()
    tags = TagField()
    source = models.URLField(verify_exists=False, null=True, blank=True)
    is_code = models.BooleanField(default=False)
    is_link = models.BooleanField(default=False)
    is_private = models.BooleanField(default=False)
    is_image = models.BooleanField(default=False)
    views_counter = models.BigIntegerField(default=0)
    parent = models.ForeignKey('Pasta', null=True)
    show_on_main = models.BooleanField(default=True)
    child_count = models.IntegerField(default=0)
    
    _cached_tags = None
    _cached_child_count = None
    
    def get_tags(self):
        return self._cached_tags

    def get_first_tag(self):
        if len(self._cached_tags) > 0:
            return self._cached_tags[0]
        else:
            return None
	    
    def get_tags_count(self):
        return len(self._cached_tags)
	    
    def __init__(self, *args, **kw):
        models.Model.__init__(self, *args, **kw)
        self._cached_tags = Tag.objects.get_for_object(self)

    def __unicode__(self):
        return self.text[:128]
	
    def get_short_source(self):
        if len(self.source) > 0:
            if len(self.source) > 64:
                return self.source[:64] + '...'
            else:
                return self.source
        else:
            return None
	    
    def get_hs_brush(self):
        for tag in self._cached_tags:
            if tag.name.startswith("hs_"):
                return tag.name.replace("hs_", "")
		
        return 'plain'
	
    def get_child_count(self):
        #if self._cached_child_count == None:
        #    self._cached_child_count = Pasta.objects.filter(root_pasta=self).count()
        #return self._cached_child_count
        return self.child_count



def pasta_post_save_handler(sender, instance, created, **kwargs):
    if isinstance(instance, Pasta) and instance.parent_id and created:
        parent_pasta = instance
        while parent_pasta.parent_id:
    	    parent_pasta = Pasta.objects.get(pk = parent_pasta.parent_id)
            parent_pasta.child_count += 1
            parent_pasta.save()

def pasta_post_delete_handler(sender, instance, **kwargs):
    if isinstance(instance, Pasta) and instance.id and instance.parent_id:
        parent_pasta = instance
        while parent_pasta.parent_id:
    	    parent_pasta = Pasta.objects.get(pk = parent_pasta.parent_id)
            parent_pasta.child_count -= 1
            parent_pasta.save()

post_save.connect(pasta_post_save_handler, Pasta)
post_delete.connect(pasta_post_delete_handler, Pasta)

