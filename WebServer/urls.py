from django.conf.urls import patterns, include, url

from django.contrib import admin
admin.autodiscover()

urlpatterns = patterns('',
    # Examples:
    url(r'^$', 'hellodjango.views.home', name='home'),
    url(r'^translate/$', 'hellodjango.views.translate', name='translate'),
    url(r'^android/$', 'hellodjango.views.android', name='android'),

    # url(r'^blog/', include('blog.urls')),

    url(r'^admin/', include(admin.site.urls)),
)
