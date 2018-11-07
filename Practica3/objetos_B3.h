//**************************************************************************
// Práctica 1 usando objetos
//**************************************************************************

#include <vector>
#include <GL/gl.h>
#include "vertex.h"
#include <stdlib.h>
#include "file_ply_stl.hpp"

using namespace std;

const float AXIS_SIZE=5000;
typedef enum{POINTS,EDGES,SOLID_CHESS,SOLID} _modo;

//*************************************************************************
// clase punto
//*************************************************************************

class _puntos3D
{
public:


	_puntos3D();
void 	draw_puntos(float r, float g, float b, int grosor);

vector<_vertex3f> vertices;
};

//*************************************************************************
// clase triángulo
//*************************************************************************

class _triangulos3D: public _puntos3D
{
public:

	_triangulos3D();
void 	draw_aristas(float r, float g, float b, int grosor);
void    draw_solido(float r, float g, float b);
void 	draw_solido_ajedrez(float r1, float g1, float b1, float r2, float g2, float b2);
void 	draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);

vector<_vertex3i> caras;
};


//*************************************************************************
// clase cubo
//*************************************************************************

class _cubo: public _triangulos3D
{
public:

	_cubo(float tam=0.5);
};


//*************************************************************************
// clase piramide
//*************************************************************************

class _piramide: public _triangulos3D
{
public:

	_piramide(float tam=0.5, float al=0.75);
};

//*************************************************************************
// clase objeto ply
//*************************************************************************

class _objeto_ply: public _triangulos3D
{
public:
   _objeto_ply();

int   parametros(char *archivo);
};

//************************************************************************
// objeto por revolución
//************************************************************************

class _rotacion: public _triangulos3D
{
public:
       _rotacion();
void  parametros_2(vector<_vertex3f> perfil1, int num1);
void  parametros(vector<_vertex3f> perfil1, int num1, int tapas);
void  tapas(vector<_vertex3f> perfil1, int num1);


vector<_vertex3f> perfil;
int num;
};


/////////////////CLASES PROPIAS//////////////////////////////////////////
//************************************************************************
// clase cilindro
//************************************************************************

class _cilindro: public _rotacion
{
public:
	int num_r;
	_vertex3f aux;
	//vector<_vertex3f> perfil;

		_cilindro(float al=0.5,float r=0.5,int num=16);

		void completeCilindo(vector<_vertex3f> perfil, int num_r);

};

//************************************************************************
// clase cono
//************************************************************************

class _cono: public _rotacion
{
public:
	int num_r;
  _vertex3f aux;
  //vector<_vertex3f> perfil;


  _cono(float al,float r,int num);

	void completeCono(vector<_vertex3f> perfil, int num_r,float al);

};

//************************************************************************
// clase esfera
//************************************************************************

class _esfera: public _rotacion
{
public:
	int num_r;
	float alfa;
  _vertex3f aux;
  //vector<_vertex3f> perfil;

  _esfera(float r=0.5,int num=16);

	void completeEsfera(vector<_vertex3f> perfil, int num_r, int r);

};
////////////////////////////////////////////////////////////////////////////





//************************************************************************
// objeto articulado: tanque
//************************************************************************

class _chasis: public _triangulos3D
{
public:
       _chasis();
void 	draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);

float altura;

protected:
_rotacion  rodamiento;
_cubo  base;
};

//************************************************************************

class _torreta: public _triangulos3D
{
public:
       _torreta();
void 	draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);

float altura;
float anchura;

protected:
_cubo  base;
_piramide parte_trasera;
};

//************************************************************************

class _tubo: public _triangulos3D
{
public:
       _tubo();
void 	draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);

protected:
_rotacion tubo_abierto; // caña del cañón
};

//************************************************************************

class _tanque: public _triangulos3D
{
public:
       _tanque();
void 	draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);

float giro_tubo;
float giro_torreta;

float giro_tubo_min;
float giro_tubo_max;

protected:
_chasis  chasis;
_torreta  torreta;
_tubo     tubo;
};



//************************************************************************
// objeto articulado: aracnido
//************************************************************************

////////////////////PATA DELANTE DERECHA////////////
class _pata_d_der: public _triangulos3D
{
public:
  _pata_d_der();
void draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);

  float altura,anchura;
	float giro_pata;
	float giro_pata_max;
	float giro_pata_min;

protected:
  _cilindro pata_d_der;
};

////////////////////////PATA DELANTERA IZQUIERDA////////////
class _pata_d_izq: public _triangulos3D
{
public:
  _pata_d_izq();
void draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);

  float altura,anchura;

protected:
  _cilindro pata_d_izq;
};

///////////////////////////PATAS TRASERAS//////////////////////
class _pata_t_der: public _triangulos3D
{
public:
	_pata_t_der();
	void draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);
	float giro_pata;
	float giro_pata_max;
	float giro_pata_min;

	protected:
	_cubo pata_t_der;

};
///////////////////////////PATAS TRASERAS//////////////////////
class _pata_t_izq: public _triangulos3D
{
public:
	_pata_t_izq();
void draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);
	float giro_pata;
	float giro_pata_max;
	float giro_pata_min;

	protected:
	_cubo pata_t_izq;

};
/////////////////////////////OJO ////////////////////////////////////
class _ojo: public _triangulos3D
{
public:
	_ojo();
void draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);
	float giro_ojo;
	float giro_ojo_max;
	float giro_ojo_min;

protected:
	_esfera ojo;

};


////////////////////////////////CUERPO COMPLETO///////////////////
class _cuerpo: public _triangulos3D
{
public:
  _cuerpo();
void draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);

  float altura,anchura;
	float giro_cabeza;
	float giro_cabeza_max;
	float giro_cabeza_min;
	float giro_pata;
	float giro_pata_max;
	float giro_pata_min;
	float giro_cuerpo;
	float giro_cuerpo_max;
	float giro_cuerpo_min;
	float giro_abdomen;
	float giro_abdomen_max;
	float giro_abdomen_min;

protected:
  _cubo cabeza;
  _cilindro cuello;
	_cubo abdomen;
	_pata_d_der pata_d_der,pata_d_der2;
	_pata_d_izq pata_d_izq,pata_d_izq2;
	_pata_t_der pata_t_der,pata_t_der2;
	_pata_t_izq pata_t_izq,pata_t_izq2;
	_ojo ojo,ojo2;
};
