from django.shortcuts import render_to_response
from django.contrib.auth.decorators import login_required
from django.template import RequestContext
from django.views.decorators.cache import cache_control
from django.contrib.auth import authenticate,login,logout
from django.http import HttpResponseRedirect,HttpResponse
from tdil.models import Log
import urllib
import urllib2
import time
import re
import os
#from django.template import RequestContext, loader


def translate(request):
    text = {}
    ret = {}

    try:
        text_to_translate = re.sub("[^a-zA-Z0-9 ]","", request.GET['txtSrc'])
    
    except:
        text_to_translate = re.sub("[^a-zA-Z0-9 ]","", request.POST['txtSrc'])
    
    try:
        l = Log.objects.get(ttext=text_to_translate)
        ret ['c'] = l.attext

    except:
        translated_text = os.system(echo text_to_translate | moses -f phrase-model/moses.ini > out)
        log = Log(ttext=text_to_translate , attext=translated_text)
        log.save()
        ret['c'] = translated_text
    
    return render_to_response('android-adb', ret )

def android(request):
    text = {}

    try:
        text_to_translate = re.sub("[^a-zA-Z0-9 ]","", request.GET['txtSrc'])
    
    except:
        text_to_translate = re.sub("[^a-zA-Z0-9 ]","", request.POST['txtSrc'])
    
    try:
        l = Log.objects.get(ttext=text_to_translate)
        translated_text = l.attext

    except:
        translated_text = os.system(echo text_to_translate | moses -f phrase-model/moses.ini > out)
        log = Log(ttext=text_to_translate , attext=translated_text)
        log.save()
    
    return HttpResponse(translated_text)