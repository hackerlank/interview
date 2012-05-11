from django import template

register = template.Library()

def pastacut(value):
    return value if len(value) < 1500 else value[:1500]
    
register.filter('pastacut', 'pastacut')