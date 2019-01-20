//**************************************************************************
// Práctica 1 usando objetos
//**************************************************************************

#include "objetos_B4.h"


//*************************************************************************
// _puntos3D
//*************************************************************************

_puntos3D::_puntos3D()
{
}

//*************************************************************************
// dibujar puntos
//*************************************************************************

void _puntos3D::draw_puntos(float r, float g, float b, int grosor)
{
//**** usando vertex_array ****
glPointSize(grosor);
glColor3f(r,g,b);

glEnableClientState(GL_VERTEX_ARRAY);
glVertexPointer(3,GL_FLOAT,0,&vertices[0]);
glDrawArrays(GL_POINTS,0,vertices.size());

/*int i;
glPointSize(grosor);
glColor3f(r,g,b);
glBegin(GL_POINTS);
for (i=0;i<vertices.size();i++){
	glVertex3fv((GLfloat *) &vertices[i]);
	}
glEnd();*/
}


//*************************************************************************
// _triangulos3D
//*************************************************************************

_triangulos3D::_triangulos3D(){
	r = 0.0f;
	g = 0.0f;
	b = 1.0f;
}

void _triangulos3D::draw_solido(){
	draw_solido(this->r, this->g, this->b);
}

void _triangulos3D::calcular_normales_caras(){
		int i = 0;
		printf("Normalizando caras");
		if(!b_normales_caras){
				for(vector<_vertex3i>::iterator it = caras.begin(); it != caras.end(); ++it){
					i++;
					_vertex3f x0= vertices[(*it)._0];
					_vertex3f x1= vertices[(*it)._1];
					_vertex3f x2= vertices[(*it)._2];

					//Producto vectorial de dos vectores que forman el triangulo de la cara.
					_vertex3f v1 = x1 - x0;
					_vertex3f v2 = x2 - x0;

					//Anidamos al vector de caras y lo normalizamos.
					_vertex3f normal_aux = v1.cross_product(v2);
					normales_caras.push_back(normal_aux.normalize());
				}
				this->b_normales_caras = true;
		}

}


void _triangulos3D::calcular_normales_vertices(){
	printf("Normalizando vertices");
	if (!b_normales_vertices){
		if(!b_normales_caras){
			this->calcular_normales_caras();
		}

		normales_vertices.resize(vertices.size());
		//NECESARIO INICIALIZAR PARA REALIZAR SUMA
		for(int i=0; i<vertices.size();++i){
			normales_vertices[i].x=0.0;
			normales_vertices[i].y=0.0;
			normales_vertices[i].z=0.0;
		}
		//SUMA LA NORMAL DE LAS CARAS A LA NORMAL DE LOS VERTICES DE CADA CARA
		for(int i=0; i<caras.size() ; ++i){
			normales_vertices[caras[i]._0] += normales_caras[i];
			normales_vertices[caras[i]._1] += normales_caras[i];
		  normales_vertices[caras[i]._2] += normales_caras[i];
		}
		//NORMALIZA VERTICES
		for(int i=0; i<vertices.size(); ++i){
			normales_vertices[i].normalize();
		}
		this->b_normales_vertices = true;
	}
}

//*************************************************************************
// dibujar en modo arista
//*************************************************************************

void _triangulos3D::draw_aristas(float r, float g, float b, int grosor)
{
//**** usando vertex_array ****
glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
glLineWidth(grosor);
glColor3f(r,g,b);

glEnableClientState(GL_VERTEX_ARRAY);
glVertexPointer(3,GL_FLOAT,0,&vertices[0]);
glDrawElements(GL_TRIANGLES,caras.size()*3,GL_UNSIGNED_INT,&caras[0]);

/*int i;
glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
glLineWidth(grosor);
glColor3f(r,g,b);
glBegin(GL_TRIANGLES);
for (i=0;i<caras.size();i++){
	glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
	glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
	glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
	}
glEnd();*/
}

//*************************************************************************
// dibujar en modo sólido
//*************************************************************************

void _triangulos3D::draw_solido(float r, float g, float b)
{
int i;

glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
glColor3f(r,g,b);
glBegin(GL_TRIANGLES);
for (i=0;i<caras.size();i++){
	glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
	glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
	glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
	}
glEnd();
}

//*************************************************************************
// dibujar en modo sólido con apariencia de ajedrez
//*************************************************************************

void _triangulos3D::draw_solido_ajedrez(float r1, float g1, float b1, float r2, float g2, float b2)
{
int i;
glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
glBegin(GL_TRIANGLES);
for (i=0;i<caras.size();i++){
	if (i%2==0) glColor3f(r1,g1,b1);
	else glColor3f(r2,g2,b2);
	glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
	glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
	glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
	}
glEnd();
}

//*************************************************************************
// dibujar con distintos modos
//*************************************************************************

void _triangulos3D::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor)
{
switch (modo){
	case POINTS:draw_puntos(r1, g1, b1, grosor);break;
	case EDGES:draw_aristas(r1, g1, b1, grosor);break;
	case SOLID_CHESS:draw_solido_ajedrez(r1, g1, b1, r2, g2, b2);break;
	case SOLID:draw_solido(r1, g1, b1);break;
	case SOLID_ILLUMINATED_FLAT:draw_iluminacion_plana();break;
	case SOLID_ILLUMINATED_GOURAUD:draw_iluminacion_suave();break;
	}
}

////////////////////////////////////////////////////
// iluminacion plana
////////////////////////////////////////////////////

void _triangulos3D::draw_iluminacion_plana()
{
	GLfloat ambient_component[4] = {1,1,1,1};

	if(!b_normales_caras){
		this->calcular_normales_caras();
	}

	glShadeModel(GL_FLAT);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glEnable(GL_NORMALIZE);


	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (GLfloat*)&ambiente_);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (GLfloat*)&difusa_);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (GLfloat*)&especular_);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, brillo);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glBegin(GL_TRIANGLES);
	for(int i = 0; i < caras.size(); ++i){
		glNormal3fv((GLfloat*)&normales_caras[i]);
		glVertex3fv((GLfloat*)&vertices[caras[i]._0]);
		glVertex3fv((GLfloat*)&vertices[caras[i]._1]);
		glVertex3fv((GLfloat*)&vertices[caras[i]._2]);
	}
	glEnd();
	glDisable(GL_LIGHTING);
}

////////////////////////////////////////////////////
// iluminacion suave
////////////////////////////////////////////////////

void _triangulos3D::draw_iluminacion_suave()
{
	if(normales_vertices.empty()){
		calcular_normales_vertices();
	}

	glShadeModel(GL_SMOOTH);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glEnable(GL_NORMALIZE);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glBegin(GL_TRIANGLES);



	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (GLfloat*)&ambiente_);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (GLfloat*)&difusa_);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (GLfloat*)&especular_);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, brillo);

	for (int i=0; i<caras.size() ; ++i){
		glNormal3fv((GLfloat*)&normales_vertices[caras[i]._0]);
    glVertex3fv((GLfloat*)&vertices[caras[i]._0]);
    glNormal3fv((GLfloat*)&normales_vertices[caras[i]._1]);
    glVertex3fv((GLfloat*)&vertices[caras[i]._1]);
    glNormal3fv((GLfloat*)&normales_vertices[caras[i]._2]);
    glVertex3fv((GLfloat*)&vertices[caras[i]._2]);
	}
	glEnd();

	glDisable(GL_LIGHTING);
	glDisable(GL_NORMALIZE);
}

//*************************************************************************
// clase cubo
//*************************************************************************

_cubo::_cubo(float tam)
{
//vertices
vertices.resize(8);
vertices[0].x=-tam;vertices[0].y=-tam;vertices[0].z=tam;
vertices[1].x=tam;vertices[1].y=-tam;vertices[1].z=tam;
vertices[2].x=tam;vertices[2].y=tam;vertices[2].z=tam;
vertices[3].x=-tam;vertices[3].y=tam;vertices[3].z=tam;
vertices[4].x=-tam;vertices[4].y=-tam;vertices[4].z=-tam;
vertices[5].x=tam;vertices[5].y=-tam;vertices[5].z=-tam;
vertices[6].x=tam;vertices[6].y=tam;vertices[6].z=-tam;
vertices[7].x=-tam;vertices[7].y=tam;vertices[7].z=-tam;

// triangulos
caras.resize(12);
caras[0]._0=0;caras[0]._1=1;caras[0]._2=3;
caras[1]._0=3;caras[1]._1=1;caras[1]._2=2;
caras[2]._0=1;caras[2]._1=5;caras[2]._2=2;
caras[3]._0=5;caras[3]._1=6;caras[3]._2=2;
caras[4]._0=5;caras[4]._1=4;caras[4]._2=6;
caras[5]._0=4;caras[5]._1=7;caras[5]._2=6;
caras[6]._0=0;caras[6]._1=7;caras[6]._2=4;
caras[7]._0=0;caras[7]._1=3;caras[7]._2=7;
caras[8]._0=3;caras[8]._1=2;caras[8]._2=7;
caras[9]._0=2;caras[9]._1=6;caras[9]._2=7;
caras[10]._0=0;caras[10]._1=1;caras[10]._2=4;
caras[11]._0=1;caras[11]._1=5;caras[11]._2=4;
}


//*************************************************************************
// clase piramide
//*************************************************************************

_piramide::_piramide(float tam, float al)
{

//vertices
vertices.resize(5);
vertices[0].x=-tam;vertices[0].y=0;vertices[0].z=tam;
vertices[1].x=tam;vertices[1].y=0;vertices[1].z=tam;
vertices[2].x=tam;vertices[2].y=0;vertices[2].z=-tam;
vertices[3].x=-tam;vertices[3].y=0;vertices[3].z=-tam;
vertices[4].x=0;vertices[4].y=al;vertices[4].z=0;

caras.resize(6);
caras[0]._0=0;caras[0]._1=1;caras[0]._2=4;
caras[1]._0=1;caras[1]._1=2;caras[1]._2=4;
caras[2]._0=2;caras[2]._1=3;caras[2]._2=4;
caras[3]._0=3;caras[3]._1=0;caras[3]._2=4;
caras[4]._0=3;caras[4]._1=1;caras[4]._2=0;
caras[5]._0=3;caras[5]._1=2;caras[5]._2=1;
}




//************************************************************************
// clase cilindro
//************************************************************************

_cilindro::_cilindro(float al,float r,int num)
{

	aux.x=r; aux.y=-al; aux.z=0;
	perfil.push_back(aux);
	aux.x=r; aux.y=al; aux.z=0;
	perfil.push_back(aux);


	num_r = num;
	_rotacion::parametros_2(perfil,num_r);
	_rotacion::tapas(perfil,num_r);


}

//*************************************************************************
// clase cono
//*************************************************************************
_cono::_cono(float al, float r,int num)
{
	aux.x=r; aux.y=(-al/2); aux.z=0.0;
	perfil.push_back(aux);

	num_r = num;

	_rotacion::parametros_2(perfil,num_r);
	completeCono(perfil,num_r,al);
}


void _cono::completeCono(vector<_vertex3f> perfil, int num_r, float al){
		int i,j;
		_vertex3i cara_aux;
		_vertex3f verticeUp,centerDOWN;

		//tapa inferior
		if(fabs(perfil[0].x)>0.0){
			centerDOWN.x=0.0;
			centerDOWN.z=0.0;
			centerDOWN.y=perfil[0].y;
			vertices.push_back(centerDOWN);
				for(i=0; i<num_r*perfil.size();i+=perfil.size()){
					cara_aux._0=i;
					cara_aux._1=(perfil.size()*num_r);
					cara_aux._2=(i+perfil.size())%(num_r*perfil.size());
					caras.push_back(cara_aux);
				}
		}
		//tapa superior
		if(fabs(perfil[perfil.size()-1].x>0.0)){
			verticeUp.x=0.0;
			verticeUp.y=al/2;
			verticeUp.z=0.0;
			vertices.push_back(verticeUp);
			for(i=0; i < num_r*perfil.size() ; i+=perfil.size()){
				cara_aux._0=i;
				cara_aux._1=(perfil.size()*num_r+1);
				cara_aux._2=(i+perfil.size())%(num_r*perfil.size());
				caras.push_back(cara_aux);
			}
		}

}


//*************************************************************************
// clase esfera
//*************************************************************************
_esfera::_esfera(float r,int num)
{


	for(int i=1; i<=num ; i++){
		alfa = -(M_PI/2) + (i*M_PI/(num+1));
		aux.x=r*cos(alfa);
		aux.y=r*sin(alfa);
		aux.z=0.0;
		perfil.push_back(aux);
	}

	num_r = num;

	_rotacion::parametros_2(perfil,num_r);
	completeEsfera(perfil,num_r,r);
}

void _esfera::completeEsfera(vector<_vertex3f> perfil, int num_r,int r){
		int i,j;
		_vertex3i cara_aux;
		_vertex3f centerUP,centerDOWN;


		//tapa inferior
		if(fabs(perfil[0].x)>0.0){
			centerDOWN.x=0.0;
			centerDOWN.z=0.0;
			centerDOWN.y=-r;
			vertices.push_back(centerDOWN);
				for(i=0; i<num_r*perfil.size();i+=perfil.size()){
					cara_aux._0=i;
					cara_aux._1=(perfil.size()*num_r);
					cara_aux._2=(i+perfil.size())%(num_r*perfil.size());
					caras.push_back(cara_aux);
				}
		}
		//tapa superior
		if(fabs(perfil[perfil.size()-1].x>0.0)){
			centerUP.x=0.0;
			centerUP.z=0.0;
			centerUP.y=r;
			vertices.push_back(centerUP);
			for(i=perfil.size()-1;i<num_r*perfil.size();i+=perfil.size()){
				cara_aux._0=(i+perfil.size())%(num_r*perfil.size());
				cara_aux._1=(perfil.size()*num_r+1);
				cara_aux._2=i;
				caras.push_back(cara_aux);
			}
		}

}










//*************************************************************************
// clase objeto ply
//*************************************************************************

/*
_objeto_ply::_objeto_ply()
{
   // leer lista de coordenadas de vértices y lista de indices de vértices

}



int _objeto_ply::parametros(char *archivo)
{
int n_ver,n_car;

vector<float> ver_ply ;
vector<int>   car_ply ;

_file_ply::read(archivo, ver_ply, car_ply );

n_ver=ver_ply.size()/3;
n_car=car_ply.size()/3;

printf("Number of vertices=%d\nNumber of faces=%d\n", n_ver, n_car);

//if (n_ver<3 || n_car<1){
//	printf("Error %d %d\n",__FILE__,__LINE__);
//	exit(-1);
//	}

vertices.resize(n_ver);
caras.resize(n_car);

_vertex3f ver_aux;
_vertex3i car_aux;

for (int i=0;i<n_ver;i++)
	{ver_aux.x=ver_ply[i*3];
	 ver_aux.y=ver_ply[i*3+1];
	 ver_aux.z=ver_ply[i*3+2];
	 vertices[i]=ver_aux;
	}

for (int i=0;i<n_car;i++)
	{car_aux.x=car_ply[i*3];
	 car_aux.y=car_ply[i*3+1];
	 car_aux.z=car_ply[i*3+2];
	 caras[i]=car_aux;
	}

return(0);
}
*/

//************************************************************************
// objeto por revolucion
//************************************************************************

_rotacion::_rotacion()
{

}

///ROTACION ANTERIOR////////////

void _rotacion::parametros_2(vector<_vertex3f> perfil, int num)
{
int i,j;
_vertex3f vertice_aux;
_vertex3i cara_aux;
int num_aux;
//_vertex3f centerUP,centerDOWN;
// tratamiento de los vértice

num_aux=perfil.size();
vertices.resize((num_aux*num));

for (j=0;j<num;j++)
  {for (i=0;i<num_aux;i++)
     {
      vertice_aux.x=perfil[i].x*cos(2.0*M_PI*j/(1.0*num))+
                    perfil[i].z*sin(2.0*M_PI*j/(1.0*num));
      vertice_aux.z=-perfil[i].x*sin(2.0*M_PI*j/(1.0*num))+
                    perfil[i].z*cos(2.0*M_PI*j/(1.0*num));
      vertice_aux.y=perfil[i].y;
      vertices[i+j*num_aux]=vertice_aux;
     }
  }

// tratamiento de las caras

for (j=0;j<num;j++)
  {for (i=0;i<num_aux-1;i++)								//Primera vuelta    1------3
     {cara_aux._0=i+((j+1)%num)*num_aux;		// valor= 2					|	\	  |
      cara_aux._1=i+1+((j+1)%num)*num_aux;  // valor= 3         |		\ |
      cara_aux._2=i+1+j*num_aux;						// valor= 1					0-----2
      caras.push_back(cara_aux);

      cara_aux._0=i+1+j*num_aux;						// valor= 1
      cara_aux._1=i+j*num_aux;							// valor= 0
      cara_aux._2=i+((j+1)%num)*num_aux;    // valor= 2
      caras.push_back(cara_aux);
     }
  }
}


////////////////////////////////
void _rotacion::parametros(vector<_vertex3f> perfil, int num, int tapa)
{
int i,j;
_vertex3f vertice_aux;
_vertex3i cara_aux;
int num_aux;

// tratamiento de los vértice

num_aux=perfil.size();
vertices.resize(num_aux*num);
for (j=0;j<num;j++)
  {for (i=0;i<num_aux;i++)
     {
      vertice_aux.x=perfil[i].x*cos(2.0*M_PI*j/(1.0*num))+
                    perfil[i].z*sin(2.0*M_PI*j/(1.0*num));
      vertice_aux.z=-perfil[i].x*sin(2.0*M_PI*j/(1.0*num))+
                    perfil[i].z*cos(2.0*M_PI*j/(1.0*num));
      vertice_aux.y=perfil[i].y;
      vertices[i+j*num_aux]=vertice_aux;
     }
  }

// tratamiento de las caras

for (j=0;j<num;j++)
  {for (i=0;i<num_aux-1;i++)
     {cara_aux._0=i+((j+1)%num)*num_aux;
      cara_aux._1=i+1+((j+1)%num)*num_aux;
      cara_aux._2=i+1+j*num_aux;
      caras.push_back(cara_aux);

      cara_aux._0=i+1+j*num_aux;
      cara_aux._1=i+j*num_aux;
      cara_aux._2=i+((j+1)%num)*num_aux;
      caras.push_back(cara_aux);
     }
  }

 // tapa inferior
if (fabs(perfil[0].x)>0.0 && tapa==1)
  {
  }

 // tapa superior
 if (fabs(perfil[num_aux-1].x)>0.0 && tapa==1)
  {
  }
}

void _rotacion::tapas(vector<_vertex3f> perfil, int num){
	int i,j;
	_vertex3i cara_aux;
	_vertex3f centerDOWN,centerUP;

	//tapa inferior
	if(fabs(perfil[0].x)>0.0){
		centerDOWN.x=0.0;
		centerDOWN.z=0.0;
		centerDOWN.y=perfil[0].y;
		vertices.push_back(centerDOWN);
			for(i=0; i<num*perfil.size();i+=perfil.size()){
				cara_aux._0=i;
				cara_aux._1=(perfil.size()*num);
				cara_aux._2=(i+perfil.size())%(num*perfil.size());
				caras.push_back(cara_aux);
			}
	}
	//tapa superior
	if(fabs(perfil[perfil.size()-1].x>0.0)){
		centerUP.x=0.0;
		centerUP.z=0.0;
		centerUP.y=perfil[perfil.size()-1].y;
		vertices.push_back(centerUP);
		for(i=perfil.size()-1;i<num*perfil.size();i+=perfil.size()){
			cara_aux._0=i;
			cara_aux._1=(perfil.size()*num+1);
			cara_aux._2=(i+perfil.size())%(num*perfil.size());
			caras.push_back(cara_aux);
		}
	}
}






//************************************************************************
// objeto articulado: tanque
//************************************************************************

_chasis::_chasis()
{
// perfil para un cilindro
vector<_vertex3f> perfil;
_vertex3f aux;
aux.x=0.107;aux.y=-0.5;aux.z=0.0;
perfil.push_back(aux);
aux.x=0.107;aux.y=0.5;aux.z=0.0;
perfil.push_back(aux);
rodamiento.parametros(perfil,12,1);
altura=0.22;
};

void _chasis::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor)
{
glPushMatrix();
glScalef(1.0,0.22,0.95);
base.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
glPopMatrix();

glPushMatrix();
glRotatef(90.0,1,0,0);
rodamiento.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
glPopMatrix();

glPushMatrix();
glTranslatef(-0.25,0.0,0.0);
glRotatef(90.0,1,0,0);
rodamiento.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
glPopMatrix();

glPushMatrix();
glTranslatef(-0.5,0.0,0.0);
glRotatef(90.0,1,0,0);
rodamiento.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
glPopMatrix();

glPushMatrix();
glTranslatef(0.25,0.0,0.0);
glRotatef(90.0,1,0,0);
rodamiento.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
glPopMatrix();

glPushMatrix();
glTranslatef(0.5,0.0,0.0);
glRotatef(90.0,1,0,0);
rodamiento.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
glPopMatrix();
}

//************************************************************************

_torreta::_torreta()
{
altura=0.18;
anchura=0.65;
};

void _torreta::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor)
{
glPushMatrix();
glScalef(0.65,0.18,0.6);
base.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
glPopMatrix();

glPushMatrix();
glTranslatef(-0.325,0,0);
glRotatef(90.0,0,0,1);
glScalef(0.18,0.16,0.6);
parte_trasera.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
glPopMatrix();
}

//************************************************************************

_tubo::_tubo()
{
// perfil para un cilindro
vector<_vertex3f> perfil;
_vertex3f aux;
aux.x=0.04;aux.y=-0.4;aux.z=0.0;
perfil.push_back(aux);
aux.x=0.04;aux.y=0.4;aux.z=0.0;
perfil.push_back(aux);
tubo_abierto.parametros(perfil,12,0);
};

void _tubo::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor)
{

glPushMatrix();
glTranslatef(0.4,0,0);
glRotatef(90.0,0,0,1);
tubo_abierto.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
glPopMatrix();
}


//************************************************************************

_tanque::_tanque()
{
giro_tubo=2.0;
giro_torreta=0.0;
giro_tubo_min=-9;
giro_tubo_max=20;
};

void _tanque::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor)
{
glPushMatrix();
chasis.draw(modo, r1, g1, b1, r2, g2, b2, grosor);

glRotatef(giro_torreta,0,1,0);
glPushMatrix();
glTranslatef(0.0,(chasis.altura+torreta.altura)/2.0,0.0);
torreta.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
glPopMatrix();

glPushMatrix();
glTranslatef(torreta.anchura/2.0,(chasis.altura+torreta.altura)/2.0,0.0);
glRotatef(giro_tubo,0,0,1);
tubo.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
glPopMatrix();
glPopMatrix();

};


//////////////////////////////ARACNIDO////////////////////////////

/////////////PATA DELANTERA DERECHA////////////////
_pata_d_der::_pata_d_der(){};
void _pata_d_der::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor)
{

glPushMatrix(); //DELANTE DERECHA
	glTranslatef(0.0,0.0,0.0);
	glRotatef(90.0 , 1.0 , 0.0 , -0.5);
	glScalef(0.2,1.5,0.2);
	pata_d_der.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
glPopMatrix();

}


////////////////////PATA DELANTERA IZQUIERDA////////////
_pata_d_izq::_pata_d_izq(){};
void _pata_d_izq::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor)
{

	glPushMatrix(); //DELANTE IZQUIERDA
		glTranslatef(0.0,0.0,0.0);
		glRotatef(90.0 , -1.0 , 0.0 , -0.5);
		glScalef(0.2,1.5,0.2);
		pata_d_izq.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
	glPopMatrix();

}


/////////////////////////////PATAS TRASERAS////////////////////////////
_pata_t_der::_pata_t_der(){};
void _pata_t_der::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor)
{

glPushMatrix(); //TRASERA DERECHA
	glTranslatef(0.0,0.0,0.0);
	glRotatef(90.0 , 1.0 , 0.0 , 0.5);
	glScalef(0.2,1.5,0.2);
		pata_t_der.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
glPopMatrix();

}

_pata_t_izq::_pata_t_izq(){};
void _pata_t_izq::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor)
{

glPushMatrix(); //TRASERA IZQUIERDA
	glTranslatef(0.0,0.0,0.0);
	glRotatef(90.0 , 1.0 , 0.0 , -0.5);
	glScalef(0.2,1.5,0.2);
		pata_t_izq.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
glPopMatrix();

}

////////////////////////////////////OJO////////////////////////////////
_ojo::_ojo(){};
void _ojo::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor)
{
glPushMatrix();
	glTranslatef(0.0,0.0,0.0);
	glScalef(0.3,0.3,0.3);
		ojo.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
glPopMatrix();
}


///////////////////////////////////CUERPO COMPLETO ARACNIDO///////////////

_cuerpo::_cuerpo(){
giro_cabeza=1;
giro_cabeza_min=-20;
giro_cabeza_max=20;

giro_pata=1;
giro_pata_max=-10;
giro_pata_max=10;

giro_cuerpo=1;
giro_cuerpo_min=-360;
giro_cuerpo_max=360;

giro_abdomen=1;
giro_abdomen_max=0;
giro_abdomen_min=-10;
};

void _cuerpo::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor)
{
glPushMatrix();//CUERPO TOTAL
	glTranslatef(0.0,0.0,0.0);
	glRotatef(giro_cuerpo,0.0,1.0,0.0);
			glPushMatrix();	//CUELLO
				glTranslatef(0.0,0.0,0.0);
				glRotatef(90.0,0.0,0.0,1.0);
				glScalef(0.40,0.65,0.40);
				cuello.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
			glPopMatrix();





			glPushMatrix();//MOVIMIENTO CABEZA
				glTranslatef(0.83,0.0,0.0);
				glRotatef(giro_cabeza,0.0,1.0,0.0);
					glPushMatrix();	//CABEZA
						glTranslatef(0.0,0.0,0.0);
						glScalef(1.0,0.8,1.0);
						cabeza.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
					glPopMatrix();
					
			glPopMatrix();



			glPushMatrix();	//ABDOMEN
				glTranslatef(-0.5,0.2,0.0);
				glRotatef(giro_abdomen,0.0,0.0,1.0);
				glPushMatrix();
					glTranslatef(-0.5,0.0,0.0);
					glScalef(1.5,1.5,1.5);
					cabeza.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
				glPopMatrix();
			glPopMatrix();

			///////////////////PATAS DELANTERAS///////////////
			glPushMatrix();//MOVIMIENTO PATA DERECHA
				glTranslatef(0.5,0.0,0.8);
				glRotatef(giro_pata,0.0,1.0,0.0);
				glPushMatrix(); //PATA DELANTERA DERECHA
					pata_d_der.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
				glPopMatrix();
				glPushMatrix(); //PATA DELANTERA DERECHA 2
					glTranslatef(0.5,-0.3,0.7);
					glRotatef(90.0,1.0,0.0,0.1);
					glScalef(0.5,0.5,0.5);
					pata_d_der2.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
				glPopMatrix();
			glPopMatrix();

			glPushMatrix();//MOVIMIENTO PATA IZQUIERDA
				glTranslatef(0.5,0.0,-0.8);
				glRotatef(giro_pata,0.0,-1.0,0.0);
				glPushMatrix(); //PATA DELANTERA IZQUIERDA
					pata_d_izq.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
				glPopMatrix();
				glPushMatrix(); //PATA DELANTERA IZQUIERDA 2
					glTranslatef(0.4,-0.3,-0.6);
					glRotatef(90.0,1.0,0.0,0.9);
					glScalef(0.5,0.5,0.5);
					pata_d_izq2.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
				glPopMatrix();
			glPopMatrix();

			////////////////////////PATAS TRASERAS//////////////////

			glPushMatrix();//MOVIMIENTO PATA TRASERA
				glTranslatef(-0.2,0.0,0.6);
				glRotatef(giro_pata,0.0,1.0,0.0);
				glPushMatrix(); //PATA TRASERA
					pata_t_der.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
				glPopMatrix();
				glPushMatrix(); //PATA TRASERA
					glTranslatef(-0.4,-0.3,0.6);
					glRotatef(90.0,1.0,0.0,0.1);
					glScalef(0.5,0.5,0.5);
					pata_t_der2.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
				glPopMatrix();
			glPopMatrix();

			glPushMatrix();//MOVIMIENTO PATA TRASERA
				glTranslatef(-0.2,0.0,-0.6);
				glRotatef(giro_pata,0.0,-1.0,0.0);
				glPushMatrix(); //PATA TRASERA
					pata_t_izq.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
				glPopMatrix();
				glPushMatrix(); //PATA TRASERA
					glTranslatef(-0.4,-0.3,-0.6);
					glRotatef(90.0,-1.0,0.0,0.1);
					glScalef(0.5,0.5,0.5);
					pata_t_izq.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
				glPopMatrix();
			glPopMatrix();

glPopMatrix();
}

///////////////////////////////////////////////////
//clase luz
///////////////////////////////////////////////////

void luz::crear(GLfloat luz_especular_2[],GLfloat luz_difu_2[],GLfloat luz_ambient_2[],GLfloat luz_posi_2[])
{

	/*glLightfv(GL_LIGHT0,GL_POSITION,luz_posi);
	glLightfv(GL_LIGHT0,GL_AMBIENT,luz_ambient);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,luz_difu);
	glLightfv(GL_LIGHT0,GL_SPECULAR,luz_especular);*/

	glEnable(GL_LIGHT1);

	//glLightfv(GL_LIGHT1,GL_AMBIENT,luz_ambient_2);
	glLightfv(GL_LIGHT1,GL_DIFFUSE,luz_difu_2);
	glLightfv(GL_LIGHT1,GL_SPECULAR,luz_especular_2);
	glLightfv(GL_LIGHT1,GL_POSITION,luz_posi_2);



};

void luz::otra_luz(GLfloat luz_especular[],GLfloat luz_difu[],GLfloat luz_ambien[],GLfloat luz_posi[])
{

	glLightfv(GL_LIGHT2,GL_POSITION,luz_posi);
	//glLightfv(GL_LIGHT2,GL_AMBIENT,luz_ambient_2);
	glLightfv(GL_LIGHT2,GL_DIFFUSE,luz_difu);
	glLightfv(GL_LIGHT2,GL_SPECULAR,luz_especular);
	glEnable(GL_LIGHT2);

}

void luz::ON(GLenum l)
{
	glEnable(l);

}

void luz::OFF(GLenum l)
{
	glDisable(l);

}
void luz::move(float m, char movimiento)
{
	cout << horizontal <<endl;

	if(movimiento == 'h'){  // horizontal
		horizontal+=m;
	}
	else if(movimiento== 'v'){ //vertical
		vertical+=m;
	}

	if(horizontal > 360)
		horizontal -=360;
	else if(horizontal < -360)
		horizontal +=360;
	if(vertical > 360)
		vertical -=360;
	else if(vertical < -360)
		vertical +=360;

	glPushMatrix();
		glRotatef(vertical,0,1,0);
		glRotatef(horizontal, 1,0,0);
		glLightfv(GL_LIGHT1, GL_POSITION, luz_posi_2); //luz_punto cambia al pulsar las teclas
	glPopMatrix();
}

void luz::estatica(GLfloat posicion_estatica[])
{
	//GLfloat posicion_estatica[] = {-10.0,-10.0,10.0,0.0};

	 glPushMatrix();
			 glRotatef(vertical,0,1,0);
			 glRotatef(horizontal,1,0,0);
			 glLightfv(GL_LIGHT1, GL_POSITION, posicion_estatica);
	 glPopMatrix();

}


/*
///////TEXTURAS
_textura::_textura(){

};



void _textura::ready_text()
{

    logo.load("./skybox_p.jpg");

    // empaquetamos bien los datos
    for (long y = 0; y < logo.height(); y ++)
        for (long x = 0; x < logo.width(); x ++)
        {
          unsigned char *r = logo.data(x, y, 0, 0);
          unsigned char *g = logo.data(x, y, 0, 1);
          unsigned char *b = logo.data(x, y, 0, 2);
          data.push_back(*r);
          data.push_back(*g);
          data.push_back(*b);
        }

		glGenTextures(1, &textura_id);

		glBindTexture(GL_TEXTURE_2D, textura_id);

   	glActiveTexture(GL_TEXTURE0);
   	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		cout << "textura LISTA)"<<endl;
   	// TRASFIERE LOS DATOS A GPU
   	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, logo.width(), logo.height(),
									0, GL_RGB, GL_UNSIGNED_BYTE, &data[0]);

};

void _textura::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor)
{
switch (modo){
	case PURE_TEXT:draw_text();break;
	case SOLID_ILLUMINATED_FLAT:draw_iluminacion_text_plana();break;
	case SOLID_ILLUMINATED_GOURAUD:draw_iluminacion_text_suave();break;
	}
}


void _textura::draw_text()
{

	glEnable(GL_TEXTURE_2D);
   glEnableClientState(GL_VERTEX_ARRAY);
   glEnableClientState (GL_TEXTURE_COORD_ARRAY_EXT);

   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, textura_id);

	glVertexPointer(3, GL_FLOAT, 0, vertices);
    glTexCoordPointer(2, GL_FLOAT, 0, texVertices);

    glDrawArrays(GL_QUADS, 0, 24);

   glDisableClientState(GL_VERTEX_ARRAY);
   glBindTexture(GL_TEXTURE_2D, 0);
   glDisable(GL_TEXTURE_2D);
}

void _textura::draw_iluminacion_text_plana()
{


	GLfloat ambient_component[4] = {1,1,1,1};

	if(!b_normales_caras){
		this->calcular_normales_caras();
	}

	glShadeModel(GL_FLAT);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glEnable(GL_NORMALIZE);


	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (GLfloat*)&ambiente_);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (GLfloat*)&difusa_);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (GLfloat*)&especular_);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, brillo);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glBegin(GL_TRIANGLES);

	for(int i = 0; i < caras.size(); ++i){
		glNormal3fv((GLfloat*)&normales_caras[i]);
		glVertex3fv((GLfloat*)&vertices[caras[i]._0]);
		glVertex3fv((GLfloat*)&vertices[caras[i]._1]);
		glVertex3fv((GLfloat*)&vertices[caras[i]._2]);
	}
	glEnd();
	glDisable(GL_LIGHTING);

	///// parte de texturas

	draw_text();

}

void _textura::draw_iluminacion_text_suave()
{
	if(normales_vertices.empty()){
		calcular_normales_vertices();
	}

	glShadeModel(GL_SMOOTH);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glEnable(GL_NORMALIZE);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glBegin(GL_TRIANGLES);



	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (GLfloat*)&ambiente_);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (GLfloat*)&difusa_);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (GLfloat*)&especular_);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, brillo);

	for (int i=0; i<caras.size() ; ++i){
		glNormal3fv((GLfloat*)&normales_vertices[caras[i]._0]);
    glVertex3fv((GLfloat*)&vertices[caras[i]._0]);
    glNormal3fv((GLfloat*)&normales_vertices[caras[i]._1]);
    glVertex3fv((GLfloat*)&vertices[caras[i]._1]);
    glNormal3fv((GLfloat*)&normales_vertices[caras[i]._2]);
    glVertex3fv((GLfloat*)&vertices[caras[i]._2]);
	}
	glEnd();

	glDisable(GL_LIGHTING);
	glDisable(GL_NORMALIZE);

	///// parte de texturas

	draw_text();

}

void _textura::release_text()
{

	glDeleteTextures(1, &textura_id);
}

void _textura::calcular_normales_caras(){
		int i = 0;

		if(!b_normales_caras){
				for(vector<_vertex3i>::iterator it = caras.begin(); it != caras.end(); ++it){
					i++;
					_vertex3f x0= vertices[(*it)._0];
					_vertex3f x1= vertices[(*it)._1];
					_vertex3f x2= vertices[(*it)._2];

					//Producto vectorial de dos vectores que forman el triangulo de la cara.
					_vertex3f v1 = x1 - x0;
					_vertex3f v2 = x2 - x0;

					//Anidamos al vector de caras y lo normalizamos.
					_vertex3f normal_aux = v1.cross_product(v2);
					normales_caras.push_back(normal_aux.normalize());
				}
				this->b_normales_caras = true;
		}

}


void _textura::calcular_normales_vertices(){

	if (!b_normales_vertices){
		if(!b_normales_caras){
			this->calcular_normales_caras();
		}

		normales_vertices.resize(72);
		//NECESARIO INICIALIZAR PARA REALIZAR SUMA
		for(int i=0; i<72;++i){
			normales_vertices[i].x=0.0;
			normales_vertices[i].y=0.0;
			normales_vertices[i].z=0.0;
		}
		//SUMA LA NORMAL DE LAS CARAS A LA NORMAL DE LOS VERTICES DE CADA CARA
		for(int i=0; i<caras.size() ; ++i){
			normales_vertices[caras[i]._0] += normales_caras[i];
			normales_vertices[caras[i]._1] += normales_caras[i];
		  normales_vertices[caras[i]._2] += normales_caras[i];
		}
		//NORMALIZA VERTICES
		for(int i=0; i<72; ++i){
			normales_vertices[i].normalize();
		}
		this->b_normales_vertices = true;
	}
}
*/