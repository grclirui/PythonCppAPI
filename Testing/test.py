def simple():
    print "Simple !"

def multiply(a,b):
    "Multipies the other way round!"
    c = 0
    for i in range(0, a):
        c = c + b
    return c

def echo(s):
    "echo!!"
    return s

def names_as_list():
    return ['Haha', 'Lala', 'Tata'];

def names_as_tuple():
    return ('Haha', 'Lala', 'Tata');

def config():
    return {"name" : "lala", "id" : 345, "key" : "7heaven",
            "email" : ("lala_land@mailsvr.com", "lala_land@mailsvr.com") }
