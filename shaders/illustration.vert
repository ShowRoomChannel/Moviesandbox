attribute float vertexID;

uniform float time;
uniform float particleMultiplier;
uniform float particleAngleScale;
uniform float objectID;
uniform float farClip;


varying float zPos;
varying float oID;

/*
*   Point Size
*/

float pointSize(){

  float particleScale=  gl_Vertex.w *  particleMultiplier * gl_Position.w ;
  particleScale+=  particleAngleScale * (1.0 - abs(gl_Normal.z));
  particleScale+=  particleAngleScale * (abs(gl_Normal.y ));
  if (gl_Position.z>0.3){
      if (gl_Position.z<1.0 )
        return ( (particleScale * 1000.0  ) / (gl_Position.z) );

      return ( (particleScale * 1000.0  ) / (gl_Position.z * gl_Position.z) );
  }else
  return 1.0;

}

/*
*   Main
*/

void main(){

  gl_FrontColor=gl_Color;

  //reset gl_Vertex coordinate or we create weird distortions!
  vec4 myVertex=gl_Vertex;
  myVertex.w=1.0;

  float xC=myVertex.x;
  myVertex.x+=0.005*myVertex.y*sin(1.0*sin(0.01*myVertex.y+0.0025* time));
  myVertex.z+=0.0025*cos(xC+0.008* time);
  myVertex.y+=0.025*sin(myVertex.z+0.008* time);

  gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * myVertex;

  gl_PointSize= pointSize() * (1.15+0.15* sin(0.001* time + objectID +xC+myVertex.y));

    zPos=gl_Position.z/farClip;
    oID= (objectID+100.0) /1024.0;


}




