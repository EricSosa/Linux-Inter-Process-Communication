#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <math.h>
#include <unistd.h>

using namespace std;

struct point
{
  int x;
  int y;
};

struct line
{
  point src;
  point dst;
  std::string name;
};

vector<line> generatesegments(vector<point> coords, std::string name)
{
  vector<line> segments;
  segments.clear();
  for (unsigned int i = 0; i < coords.size() - 1; i++)
  {
    line newline;
    newline.src = coords[i];
    newline.dst = coords[i + 1];
    newline.name = name;
    segments.push_back(newline);
  }
  return segments;
}
float distance(point a, point b)
{
  return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}
bool intersect(line l1, line l2)
{
  bool inter;
  //"""Returns a point at which two lines intersect"""
  int x1 = l1.src.x;
  int y1 = l1.src.y;
  int x2 = l1.dst.x;
  int y2 = l1.dst.y;

  int x3 = l2.src.x;
  int y3 = l2.src.y;
  int x4 = l2.dst.x;
  int y4 = l2.dst.y;

  float tnum;
  tnum = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4));
  float tden;
  tden = ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));
  if (tden == 0)
    return inter = false;
  //"""returns if parallel line"""
  float t;
  t = tnum / tden;

  float unum;
  unum = ((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3));
  float uden;
  uden = ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));
  if (uden == 0)
    return inter = false;
  //"""returns if parallel line"""
  float u;
  u = -(unum / uden);

  if (((0.0 <= u) && (u <= 1.0)) && ((0.0 <= t) && (t <= 1.0)))
  {
    //final = (xcooru, ycooru);
    return inter = true;
  }
  else
    return inter = false;
}

int main(int argc, char **argv)
{
  int z;
  int numbstr = 10;
  int numbseg = 5;
  int time = 5;
  int range = 20;
  string sinp;
  string ninp;
  string linp;
  string cinp;

opterr = 0;

  while ((z = getopt(argc, argv, "s:n:l:c:")) != -1)
    switch (z)
    {
    case 's':
      sinp = optarg;
      numbstr = atoi(sinp.c_str());
      break;
    case 'n':
      ninp = optarg;
      numbseg = atoi(ninp.c_str());
      break;
    case 'l':
      linp = optarg;
      time = atoi(linp.c_str());
      break;
    case 'c':
      cinp = optarg;
      range = atoi(cinp.c_str());
      break;
    case '?':
      std::cerr << "Error: unknown option: " << optopt << std::endl;
      return 0;
    default:
      return 0;
    }
  if (numbstr < 2)
  {
    std::cerr << "Error: wrong argument value" << std::endl;
    return 0;
  }
  if (numbseg < 1)
  {
    std::cerr << "Error: wrong argument value" << std::endl;
    return 0;
  }
  if (time < 5)
  {
    std::cerr << "Error: wrong argument value" << std::endl;
    return 0;
  }
  if (range < 1)
  {
    std::cerr << "Error: wrong argument value" << std::endl;
    return 0;
  }

  // open /dev/urandom to read
  std::ifstream urandom("/dev/urandom");
  if (urandom.fail())
  {
    std::cerr << "Error: cannot open /dev/urandom\n";
    return 0;
  }

  while (1)
  {
    char c = 'a';
    char ch = 'a';
    int r = 0;
    int i = 0;
    int k = 0;
    int n = 0;
    int numbstr2 = 2;
    int numbseg2 = 1;
    vector<line> segments;
    vector<line> segmentsfinal;
    int count = 0;
    bool intersection = false;
    bool overlap = false;
    vector<string> allstreets;

    //random between 2 and number of streets by user
    ch = 'a';
    urandom.read(&ch, 1);
    numbstr2 = (unsigned int)ch % numbstr + 1;
    if (numbstr2 < 2)
      numbstr2 = 2;

    for (i = 0; i < numbstr2; i++)
    {
      count = 0;
      std::string name;
      //loop to generate 11 letter name
      for (k = 0; k < 11; k++)
      {
        urandom.read(&ch, 1);
        r = (unsigned int)ch % 26; // rand number 0 - 25
        c = 'a' + r;
        name += c;
      }

      //random between 1 and number of segments by user
      ch = 'a';
      urandom.read(&ch, 1);
      numbseg2 = (unsigned int)ch % numbseg + 1;

      vector<point> coords;
      coords.clear();
      coords.push_back(point());
      ch = 'a';
      urandom.read(&ch, 1);
      coords[0].x = (int)ch % (range + 1);
      ch = 'a';
      urandom.read(&ch, 1);
      coords[0].y = (int)ch % (range + 1);

      for (n = 1; n <= numbseg2; n++)
      {
        if (count == 30)
        {
          cerr << "Error: failed to generate valid input for 30 simultaneous attempts" << endl;
          urandom.close();
          return 0;
        }

        coords.push_back(point());
        ch = 'a';
        urandom.read(&ch, 1);
        coords[n].x = (int)ch % (range + 1);

        ch = 'a';
        urandom.read(&ch, 1);
        coords[n].y = (int)ch % (range + 1);

        bool samepoint = false;

        //check its not the same coord as last time
        if (coords[n].x == coords[n-1].x && coords[n].y == coords[n-1].y)
        {
          samepoint = true;
          coords.pop_back();
          n--;
          count++;
        }
        

        if (n > 1 && !samepoint)
        {

          segments.clear();
          segments = generatesegments(coords, name);

          for (int t = 0; t < (int)segments.size() - 1; t++)
          {
            //check last segment doesnt intersect previous segments on street
            intersection = false;
            //dont check intersect with previous segment
            if (t < (int)segments.size() - 2)
              intersection = intersect(segments[t], segments.back());

            //check if last segment is colinear  with another segment
            float m1 = 0;
            float m2 = 1;
            overlap = false;
            //check denominators are not 0
            if ((segments[t].dst.x - segments[t].src.x) != 0 && (segments.back().dst.x - segments.back().src.x) != 0)
            {
              m1 = (segments[t].dst.y - segments[t].src.y) / (segments[t].dst.x - segments[t].src.x);
              m2 = (segments.back().dst.y - segments.back().src.y) / (segments.back().dst.x - segments.back().src.x);
            }
            //if same slope
            if (m1 == m2 || ((segments[t].dst.x - segments[t].src.x) == 0 && (segments.back().dst.x - segments.back().src.x) == 0))
              overlap = !(segments[t].dst.x < segments.back().src.x || segments.back().dst.x < segments[t].src.x);

            if (intersection || overlap)
            {
              coords.pop_back();
              n--;
              count++;
              break;
            }
          }
          if (!intersection && !overlap)
            count = 0;
        }
      }

      //aqui vas a crear una calle con nombre y coordenadas
      int same = 0;
      bool printed = false;
      intersection = false;
      segments.clear();
      int internumb = 0;
      int finalcount = 0;
      segments = generatesegments(coords, name);

      //if first street save it on final segments print first street
      if (i == 0)
      {
        segmentsfinal = generatesegments(coords, name);
        std::cout << "a \"" << name << "\" ";
        for (int gg = 0; gg < (int)coords.size(); gg++)
          std::cout << "(" << coords[gg].x << "," << coords[gg].y << ")";
        cout << endl;
        allstreets.push_back(name);
      }

      if (i > 0)
      {
        for (int g = 0; g < (int)segments.size(); g++)
        {
          for (int h = 0; h < (int)segmentsfinal.size(); h++)
          {
            //check no 2 segments are the same
            if (
                ((segments[g].src.x == segmentsfinal[h].src.x && segments[g].dst.x == segmentsfinal[h].dst.x) &&
                 (segments[g].src.y == segmentsfinal[h].src.y && segments[g].dst.y == segmentsfinal[h].dst.y)) ||
                ((segments[g].src.x == segmentsfinal[h].dst.x && segments[g].dst.x == segmentsfinal[h].src.x) &&
                 (segments[g].src.y == segmentsfinal[h].dst.y && segments[g].dst.y == segmentsfinal[h].src.y)))
              same++;
            //check for intersections if none has been found and there were no same segments
            if (internumb == 0 && same == 0)
            {
              intersection = false;
              intersection = intersect(segments[g], segmentsfinal[h]);
              if (intersection)
                internumb++;
            }
          }
        }
        //save and print street if there were no repeated numbers
        if (i < numbstr2 - 1 && same == 0)
        {
          segmentsfinal.insert(segmentsfinal.end(), segments.begin(), segments.end());
          std::cout << "a \"" << name << "\" ";
          for (int gg = 0; gg < (int)coords.size(); gg++)
            std::cout << "(" << coords[gg].x << "," << coords[gg].y << ")";
          cout << endl;
          allstreets.push_back(name);
          printed = true;
        }

        if (i == numbstr2 - 1 && internumb > 0 && same == 0)
        {
          segmentsfinal.insert(segmentsfinal.end(), segments.begin(), segments.end());
          std::cout << "a \"" << name << "\" ";
          for (int gg = 0; gg < (int)coords.size(); gg++)
            std::cout << "(" << coords[gg].x << "," << coords[gg].y << ")";
          cout << endl;
          allstreets.push_back(name);
          printed = true;
        }

        same = 0;

        if (!printed)
        {
          finalcount++;
          i--;
        }
        if (finalcount == 30)
        {
          cerr << "Error: failed to generate valid input for 30 simultaneous attempts" << endl;
          urandom.close();
          return 0;
        }
        printed = false;
      }
    }

    cout << "g" << endl;

    // wait!!!
    sleep(time);

    for (int gg = 0; gg < (int)allstreets.size(); gg++)
      std::cout << "r \"" << allstreets[gg] << "\"" << endl;
  }

  // close random stream
  urandom.close();
  return 0;
}
