import logging
from django.core.paginator import Paginator, InvalidPage, EmptyPage
from django.http import  HttpResponseRedirect
from django.shortcuts import render_to_response
from pasta.overview.models import Pasta
from pasta.add.models import PastaForm, PastaCommentForm

from django.template import RequestContext

from django.contrib.auth.decorators import login_required

def show_pasta(request, id):
    
    pasta_list = []
    try:
        pasta_obj = Pasta.objects.get(pk=id)
        pasta_obj.views_counter += 1
        pasta_obj.save()
        pasta_list.append(pasta_obj)
    except:
        return HttpResponseRedirect('/')
    
    paginator = Paginator(pasta_list, 50)
    page = 1
    
    pastas = paginator.page(page)

    comment_form = PastaCommentForm()
    logging.debug(comment_form)
    
    return render_to_response('base.html', {'body': 'overview.html', 'pastas': pastas, 'single': 1, 'comment_form': comment_form}, context_instance=RequestContext(request))

@login_required
def delete(request, id):

    try:
        pasta = Pasta.objects.get(pk=id)
        pasta.delete()
    except:
        pass
	
    return HttpResponseRedirect('/')
	

def index(request):

    pasta_list = Pasta.objects.filter(parent=None, show_on_main=True).order_by('-date')
    paginator = Paginator(pasta_list, 50)
    
    try:
        page = int(request.GET.get('page', 1))
    except ValueError:
        page = 1
	
    try:
        pastas = paginator.page(page)
    except (EmptyPage, InvalidPage):
        pastas = paginator.page(paginator.num_pages)

    quickform = PastaForm()

    return render_to_response('base.html', {'body': 'overview.html', 'pastas': pastas, 'quickform': quickform, 'user': request.user, 'single': 0}, context_instance=RequestContext(request))

