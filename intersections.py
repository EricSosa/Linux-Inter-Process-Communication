#!/usr/bin/env python

import re
import sys

class point(object):
    """A point in a two dimensional space"""
    def __init__(self, x , y):
        self.x = float(x)
        self.y = float(y)

    def __repr__(self):
        return '(' + pp(self.x) + ', ' + pp(self.y) + ')'


class interpoint(object):
    """A point in a two dimensional space with all info on intersection"""
    def __init__(self, p0, p1, p2, p3, p4, l1, l2):
        self.p0 = (p0)
        self.p1 = (p1)
        self.p2 = (p2)
        self.p3 = (p3)
        self.p4 = (p4)
        self.l1 = (l1)
        self.l2 = (l2)

    def __repr__(self):
        return '(' + pp(self.p0) + ', ' + pp(self.p1) + ', ' + pp(self.p2) + ', ' + pp(self.p3) + ', ' + pp(self.p4) + ')'


class line(object):
    """A line between two points"""
    def __init__(self, src, dst, streetname):
        self.src = src
        self.dst = dst
        self.streetname = streetname
        self.intersects = []

    def __repr__(self):
        return '['+ str(self.src) + '-->' + str(self.dst) + ']'


def intersect (l1, l2):
    """Returns a point at which two lines intersect"""
    x1, y1 = l1.src.x, l1.src.y
    x2, y2 = l1.dst.x, l1.dst.y

    x3, y3 = l2.src.x, l2.src.y
    x4, y4 = l2.dst.x, l2.dst.y

    tnum = ((x1-x3)*(y3-y4) - (y1-y3)*(x3-x4))
    tden = ((x1-x2)*(y3-y4) - (y1-y2)*(x3-x4))
    if tden == 0:
      return
      """returns if parallel line"""  
    t = tnum/tden

    unum = ((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3))
    uden = ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4))
    if uden == 0:
      return
      """returns if parallel line"""
    u = -(unum / uden)
    xcooru = x3 + u * (x4 - x3)
    ycooru = y3 + u * (y4 - y3)

    if ((0.0 <= u) & (u <= 1.0)) & ((0.0 <= t) & (t <= 1.0)):
      final = (xcooru, ycooru)
      l1.intersects.append(final)
      l2.intersects.append(final)
      return interpoint(final,(l1.src.x,l1.src.y),(l1.dst.x,l1.dst.y),(l2.src.x,l2.src.y),(l2.dst.x,l2.dst.y),l1,l2)

    else:
      return

def generatesegments(coords, streetname):
    segments = []
    n = len(coords)
    #return (i)
    for i in range(n-1):
      segments.append(line(coords[i],coords[i+1],streetname))
    return segments


def order_list_by_dist(mylist, x, y):
   mylist.sort(key = lambda p: ( (p[0] - x)**2 + (p[1] - y)**2 ))
   return mylist


def pp(x):
    """Returns a pretty-print string representation of a number.
       A float number is represented by an integer, if it is whole,
       and up to two decimal places if it isn't
    """
    if isinstance(x, float):        
        if x.is_integer():
            return str(int(x))
        else:
            return "{0:.2f}".format(x)
    return str(x)


def graph(segments):

    vertices = []
    intersections = []
    del vertices[:]
    del intersections[:]
    m = 0
    for i in range(len(segments)):
      k = i
      while k < len(segments):
        if segments[i].streetname != segments[k].streetname:
          intersections.append(intersect(segments[i],segments[k]))
          if intersections[m] is None:
            del intersections[m]
          else:
            if intersections[m].p0 not in vertices:
              vertices.append(intersections[m].p0)
            if intersections[m].p1 not in vertices:
              vertices.append(intersections[m].p1)
            if intersections[m].p2 not in vertices:
              vertices.append(intersections[m].p2)
            if intersections[m].p3 not in vertices:
              vertices.append(intersections[m].p3)
            if intersections[m].p4 not in vertices:
              vertices.append(intersections[m].p4)
            m +=1
        k += 1


    problems=[]
    del problems[:]
    prob=[]
    del prob[:]

    for lin in segments:
      if len(lin.intersects) > 1:
        for i in range(len(intersections)):
          if (lin.src == intersections[i].l1.src)&(lin.dst == intersections[i].l1.dst)&(lin.streetname == intersections[i].l1.streetname):
            #get the 2 closest points to the intersection within line and set them to p1 & p2
            problems.extend(lin.intersects)
            problems.append(intersections[i].p1)
            problems.append(intersections[i].p2)
            problems.append(intersections[i].p0)
            prob = list(set(problems))
            order_list_by_dist(prob, intersections[i].p1[0], intersections[i].p1[1])
            z = prob.index(intersections[i].p0)
            if z-1 >= 0:
              intersections[i].p1 = prob[z-1]
            else:
              intersections[i].p1 = prob[z]
            if z+1 < len(prob):
              intersections[i].p2 = prob[z+1]
            else:
              intersections[i].p2 = prob[z]
            del problems[:]
            del prob[:]

          if (lin.src == intersections[i].l2.src)&(lin.dst == intersections[i].l2.dst)&(lin.streetname == intersections[i].l2.streetname):
            #get the 2 closest points to the intersection within line and set them to p3 & p4
            problems.extend(lin.intersects)
            problems.append(intersections[i].p3)
            problems.append(intersections[i].p4)
            problems.append(intersections[i].p0)
            prob = list(set(problems))
            order_list_by_dist(prob, intersections[i].p3[0], intersections[i].p3[1])
            z = prob.index(intersections[i].p0)
            if z-1 >= 0:
              intersections[i].p3 = prob[z-1]
            else:
              intersections[i].p3 = prob[z]
            if z+1 < len(prob):
              intersections[i].p4 = prob[z+1]
            else:
              intersections[i].p4 = prob[z]
            del problems[:]
            del prob[:]      


    edges = []
    for inter in intersections:
      if (vertices.index(inter.p0)+1, vertices.index(inter.p1)+1) not in edges:
        edges.append((vertices.index(inter.p0)+1, vertices.index(inter.p1)+1))
      if (vertices.index(inter.p0)+1, vertices.index(inter.p2)+1) not in edges:
        edges.append((vertices.index(inter.p0)+1, vertices.index(inter.p2)+1))
      if (vertices.index(inter.p0)+1, vertices.index(inter.p3)+1) not in edges:
        edges.append((vertices.index(inter.p0)+1, vertices.index(inter.p3)+1))
      if (vertices.index(inter.p0)+1, vertices.index(inter.p4)+1) not in edges:
        edges.append((vertices.index(inter.p0)+1, vertices.index(inter.p4)+1))

    return vertices, edges


def printgraph(vertices, edges):

    vertic = []
    for vert in vertices:
      vertic.append((pp(vert[0]),pp(vert[1])))

    #Remove duplicates making a set
    ed = set([tuple(sorted(i)) for i in edges])
    edgesss = []
    for e in ed:
      if e[0] != e[1]:
        edgesss.append(e)
    #Remove point to point



    sys.stdout.write("V " + str(len(vertic)+1)+ "\n")
    sys.stdout.flush()


    sys.stdout.write("E {")
    sys.stdout.flush()
    for i in range(len(edgesss)):
      if i < len(edgesss)-1:
        sys.stdout.write("<" + pp(edgesss[i][0]) + "," + pp(edgesss[i][1])+">,")
        sys.stdout.flush()
      else:
        sys.stdout.write("<" + pp(edgesss[i][0]) + "," + pp(edgesss[i][1])+">")
        sys.stdout.flush()
    sys.stdout.write("}" + "\n")
    sys.stdout.flush()


def parse_line(inp, segments):

    command = []
    command = re.findall(r'(^a |^c |^r |^g$)', inp)
    if len(command) == 1:
      command = command[0]
    else:
      raise Exception('Error: wrong command input')

    street = None
    streetname = []
    coordenates = []
    cond = 0

    if command == 'r ':
      streetname = re.findall(r'[ ]+["][a-zA-z ]+["][ ]+$|[ ]+["][a-zA-z ]+["]$', inp)
      if len(streetname) == 1:
        street = streetname[0].strip(' ""').lower()
        for seg in segments:
          if seg.streetname == street:
            cond = 1
        if cond == 0:
          del streetname[:]
          street = None
          raise Exception('Error: street not found')
          
      else:
        del streetname[:]
        raise Exception('Error: wrong street input')


    if (command == 'a '):
      streetname = re.findall(r'[ ]+["][a-zA-z ]+["][ ]+', inp)
      if len(streetname) == 1:
        street = streetname[0].strip(' ""').lower()
        for seg in segments:
          if seg.streetname == street:
            del streetname[:]
            street = None
            raise Exception('Error: street already exists')

      else:
        del streetname[:]
        raise Exception('Error: wrong street input')

    if (command == 'c '):
      streetname = re.findall(r'[ ]+["][a-zA-z ]+["][ ]+', inp)
      if len(streetname) == 1:
        street = streetname[0].strip(' ""').lower()
        for seg in segments:
          if seg.streetname == street:
            cond = 1
        if cond == 0:
          del streetname[:]
          street = None
          raise Exception('Error: street not found')
      else:
        del streetname[:]
        raise Exception('Error: wrong street input')
        
    if (command == 'a ') | (command == 'c '):
      parentheses = []
      parentheses = re.findall(r'(\(|\))', inp)
      commas = []
      commas = re.findall(r',', inp)
      coords = []
      coords = re.findall(r'[(][-]?[0-9]+[,][-]?[0-9]+[)]', inp)
      if (len(coords)*2 == len(parentheses))&(len(coords)==len(commas)):
        for cord in coords:
          cord = cord.strip("()")
      else:
        del coords[:]
        raise Exception('Error: wrong coordinates input')

      if len(coords) < 2:
        raise Exception('Error: no coordinates found')

      if len(set(coords)) != len(coords):
        raise Exception('Error: duplicate coordinates')

      for cord in coords:
        x, y = map(float, cord.strip('()').split(','))
        coordenates.append(point(x,y))

    return command, street, coordenates


def main():
    segments = []
    while True:
        inp = sys.stdin.readline()
        if inp == '':
            break

        try:
          coordenates = []
          (cmd, streetname, coordenates) = parse_line(inp,segments)
          if cmd == 'a ':
            segments.extend(generatesegments(coordenates, streetname))
          elif cmd == 'g':
            #print intersect(segments[0],segments[1])
            for seg in segments:
              seg.intersects = []
            vertices, edges = graph(segments)
            printgraph(vertices, edges)
            #print segments
          elif cmd == 'r ':
            k = 0
            while k < len(segments):
              if segments[k].streetname == streetname:
                del segments[k]
                k -= 1
              k += 1
          elif cmd == 'c ':
            k = 0
            while k < len(segments):
              if segments[k].streetname == streetname:
                del segments[k]
                k -= 1
              k += 1
            segments.extend(generatesegments(coordenates, streetname))

        except Exception as ex:
            sys.stderr.write(str(ex) + '\n')
    sys.exit(0)

if __name__ == '__main__':
    main()
