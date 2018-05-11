#ifndef __COORD_H
#define __COORD_H
//---------------------------------------------------------------------------

struct Coord
{
  int x_;
  int y_;
  int z_;

  Coord(): x_(0), y_(0), z_(0) { }
  Coord(int x, int y, int z): x_(x), y_(y), z_(z) { }
  Coord(const Coord &p) {x_=p.x_; y_=p.y_; z_=p.z_;}

  bool operator==(const Coord &rhs) const
  {if (x_==rhs.x_ && y_==rhs.y_ && z_==rhs.z_) return true; else return false; }
  bool operator!=(const Coord &rhs)
  {return !(*this==rhs); }
};


struct GBaseWay {
	Coord* way;
	int size;
};


//---------------------------------------------------------------------------
#endif
 