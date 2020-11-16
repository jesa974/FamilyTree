
void normalize( OBJ3D* vect )
{
    float w = sqrt( vect->x * vect->x + vect->y * vect->y + vect->z * vect->z );
    vect->x /= w;
    vect->y /= w;
    vect->z /= w;
}

float distancePt(OBJ3D pt1, OBJ3D pt2){
	return sqrt((pt1.x - pt2.x)*(pt1.x - pt2.x) + (pt1.y - pt2.y)*(pt1.y - pt2.y) + (pt1.z - pt2.z)*(pt1.z - pt2.z));
}

float distanceDroite(OBJ3D pt, OBJ3D droite){
	return abs( droite.x*pt.x+ droite.y*pt.y + droite.z*pt.z)/ sqrt( droite.x * droite.x + droite.y * droite.y + droite.z * droite.z);
}
 

float getFovY(int fovx){
	float fovy;

	float aspect_ratio = lF/hF;
	fovy = 2 * atan(tan(fovx / 2) / aspect_ratio);

	return fovy;
}

OBJ3D diff3D(OBJ3D a, OBJ3D b){
	OBJ3D obj;
	obj.x = a.x - b.x;
	obj.y = a.y - b.y;
	obj.z = a.z - b.z;

	return obj;
}

OBJ3D crossProduct(OBJ3D a, OBJ3D b){
	//Produit en croix
	OBJ3D obj;
	obj.x = a.y*b.z - a.z * b.y;
	obj.y = a.z*b.x - a.x * b.z;
	obj.z = a.x*b.y - a.y * b.x;

	return obj;
}

OBJ3D scale(OBJ3D a, float l){
	//Multiplication
	OBJ3D obj;
	obj.x = a.x * l;
	obj.y = a.y * l;
	obj.z = a.z * l;

	return obj;
}

OBJ3D additionne(OBJ3D a, OBJ3D b){
	//Addition des axes
	OBJ3D obj;
	obj.x = a.x + b.x;
	obj.y = a.y + b.y;
	obj.z = a.z + b.z;

	return obj;
}

OBJ3D difference(OBJ3D a, OBJ3D b){
	//Addition des axes
	OBJ3D obj;
	obj.x = a.x - b.x;
	obj.y = a.y - b.y;
	obj.z = a.z - b.z;

	return obj;
}

OBJ3D testIntersection(float x, float y, OBJ3D pos, OBJ3D dir){
	OBJ3D obj;
	float s = - pos.z / dir.z;
	obj.x = pos.x+dir.x*s;
	obj.y = pos.y+dir.y*s;
	obj.z = 0;

	return obj;
}


void mousePicker(int x, int y){
	//POINT DE DEPART
	OBJ3D camera = Vision.eye;

	OBJ3D viewm;
	viewm = diff3D(Vision.center, camera); // 3D float vector
	normalize(&viewm);

	OBJ3D ht;
	ht = crossProduct( viewm, Vision.up ); // 3D float vector
	normalize(&ht);

	OBJ3D vt;
	vt = crossProduct( ht, viewm);// 3D float vector
	normalize(&vt);

	// convert fovy to radians 
	float rad = getFovY(FOV) * M_PI / 180;
	float vLength = tan( rad / 2 ) * 1.0; //1.0 est la distance plan proche
	float hLength = vLength * (lF / hF);

	//on possède les coordonnées viewPort : x,y
	float xW, yW;
	xW = x;
	yW = hF - y;
	//conversion de pixels écrans vers coordonées opengl (de -1 à 1)
	float xN = 2.0f*xW/lF - 1.0f;
	float yN = 2.0f*yW/hF - 1.0f;

	GLdouble proj[16], model[16];
	GLint view[4];
	glGetDoublev(GL_PROJECTION_MATRIX,proj);
	glGetDoublev(GL_MODELVIEW_MATRIX,model);
	glGetIntegerv(GL_VIEWPORT, view);

	GLdouble objx, objy, objz;
	gluUnProject(xN,yN,-1.0f,model,proj,view,&objx,&objy,&objz);

	// linear combination to compute intersection of picking ray with
	// view port plane
	OBJ3D pos;
	
	pos.x = objx + xN*hLength*ht.x + xN*vLength*vt.x;
	pos.y = objy + yN*hLength*ht.x + yN*vLength*vt.x;
	pos.z = objz + hLength*ht.z + vLength*vt.z;

	// compute direction of picking ray by subtracting intersection point
	// with camera position
	OBJ3D dir = difference(pos, camera);
	mouse3D = pos;
	mouse3D2 = dir;

	for(int i=0; i< genmax; i++){
		for(int j=0; j<tabGen[i]; j++){
				OBJ3D curr = indiv_coords[i][j].coords;
				
				if( distancePt(curr, pos) < 0.5){
					printf("selected individual [%d][%d]\n",i,j);
					indiv_chose_num = j;
					gen_chose = i;
					choppeIndividu(&rgb,&indiv_chosen,&id);
				}
			}
		}


}