/*
This file is used for everything related to maps/tables including their definition, functions etc
*/
#ifndef TABLE_H
#define TABLE_H

#define TABLE_RPM_MULTIPLIER  100
#define TABLE_LOAD_MULTIPLIER 2

/*
The 2D table can contain either 8-bit (byte) or 16-bit (int) values
The valueSize variable should be set to either 8 or 16 to indicate this BEFORE the table is used
*/
struct table2D {
  byte valueSize;
  byte xSize;

  byte *values;
  byte *axisX;

  int16_t *values16;
  int16_t *axisX16;

  //Store the last X and Y coordinates in the table. This is used to make the next check faster
  int16_t lastXMax;
  int16_t lastXMin;

  //Store the last input and output for caching
  int16_t lastInput;
  int16_t lastOutput;
  byte cacheTime; //TRacks when the last cache value was set so it can expire after x seconds. A timeout is required to pickup when a tuning value is changed, otherwise the old cached value will continue to be returned as the X value isn't changing. 
};

//void table2D_setSize(struct table2D targetTable, byte newSize);
void table2D_setSize(struct table2D, byte);

struct table3D {

  //All tables must be the same size for simplicity

  byte xSize;
  byte ySize;

  byte **values;
  int16_t *axisX;
  int16_t *axisY;

  //Store the last X and Y coordinates in the table. This is used to make the next check faster
  byte lastXMax, lastXMin;
  byte lastYMax, lastYMin;

protected:
  table3D(const byte& _size, byte* _data, byte** _values, int16_t* _x, int16_t* _y)
  : xSize(_size)
  , ySize(_size)
  , values(_values)
  , axisX(_x)
  , axisY(_y)
  {
    for (size_t i = 0; i < _size; i++)
    {
      _values[i] = &_data[i*_size];
    }
  }
};

template<size_t SIZE>
struct Table3DContainer : public table3D
{
private:
  byte _data[SIZE*SIZE];
  byte* _values[SIZE];
  int16_t _x[SIZE];
  int16_t _y[SIZE];

public:
  Table3DContainer()
  : table3D(SIZE, _data, _values, _x, _y)
  {  }
};

//void table3D_setSize(struct table3D *targetTable, byte);
void table3D_setSize(struct table3D *targetTable, byte);

/*
3D Tables have an origin (0,0) in the top left hand corner. Vertical axis is expressed first.
Eg: 2x2 table
-----
|2 7|
|1 4|
-----

(0,1) = 7
(0,0) = 2
(1,0) = 1

*/
int get3DTableValue(struct table3D *fromTable, int, int);
int table2D_getValue(struct table2D *fromTable, int);

#endif // TABLE_H
