uniform float time;
uniform float screenX;
uniform sampler2D tex;


varying vec2 texCoord;

// blur variables


    vec2 tc_offset[25];

    float PI = 3.14159265358979323846264;


float unpackToFloat(vec4 value){

	const vec4 bitSh = vec4(1.0 / (256.0 * 256.0 * 256.0), 1.0 / (256.0 * 256.0), 1.0 / 256.0, 1.0);

	return dot(value, bitSh);
}

float unpackToFloat(vec3 value){

	const vec3 bitSh = vec3(1.0 / (256.0 * 256.0), 1.0 / 256.0, 1.0);

	return dot(value, bitSh);
}

float unpackToFloat(vec2 value){

	const vec2 bitSh = vec2(1.0 / 256.0, 1.0);

	return dot(value, bitSh);
}

vec4 packToVec4(float value){

   const vec4 bitSh = vec4(256.0 * 256.0 * 256.0, 256.0 * 256.0, 256.0, 1.0);
   const vec4 bitMsk = vec4(0.0, 1.0 / 256.0, 1.0 / 256.0, 1.0 / 256.0);
   vec4 res = value * bitSh;
   res = fract(res);
   res -= res.xxyz * bitMsk;

   return res;
}

vec3 packToVec3(float value){

   const vec3 bitSh = vec3(256.0 * 256.0, 256.0, 1.0);
   const vec3 bitMsk = vec3(0.0, 1.0 / 256.0, 1.0 / 256.0);
   vec3 res = fract(value * bitSh);
   res -= res.xxy * bitMsk;

   return res;
}

vec2 packToVec2(float value){

   const vec2 bitSh = vec2(256.0, 1.0);
   const vec2 bitMsk = vec2(0.0, 1.0 / 256.0);
   vec2 res = fract(value * bitSh);
   res -= res.xx * bitMsk;


   return res;
}



vec4 blur5(sampler2D myTex,vec2 tc){

      float spread=1.0/screenX;// * min(0.0,max(4.0,unpackToFloat(texture2D(depthTex,texCoord).xy*farClip/100.0)));
      //float spread=2.0/screenX ;// * min(8.0,max(4.0,unpackToFloat(texture2D(depthTex,texCoord).xy*512/50.0)));

      tc_offset[0]=spread * vec2(-2.0,-2.0);
      tc_offset[1]=spread * vec2(-1.0,-2.0);
      tc_offset[2]=spread * vec2(0.0,-2.0);
      tc_offset[3]=spread * vec2(1.0,-2.0);
      tc_offset[4]=spread * vec2(2.0,-2.0);

      tc_offset[5]=spread * vec2(-2.0,-1.0);
      tc_offset[6]=spread * vec2(-1.0,-1.0);
      tc_offset[7]=spread * vec2(0.0,-1.0);
      tc_offset[8]=spread * vec2(1.0,-1.0);
      tc_offset[9]=spread * vec2(2.0,-1.0);

      tc_offset[10]=spread * vec2(-2.0,0.0);
      tc_offset[11]=spread * vec2(-1.0,0.0);
      tc_offset[12]=spread * vec2(0.0,0.0);
      tc_offset[13]=spread * vec2(1.0,0.0);
      tc_offset[14]=spread * vec2(2.0,0.0);

      tc_offset[15]=spread * vec2(-2.0,1.0);
      tc_offset[16]=spread * vec2(-1.0,1.0);
      tc_offset[17]=spread * vec2(0.0,1.0);
      tc_offset[18]=spread * vec2(1.0,1.0);
      tc_offset[19]=spread * vec2(2.0,1.0);

      tc_offset[20]=spread * vec2(-2.0,2.0);
      tc_offset[21]=spread * vec2(-1.0,2.0);
      tc_offset[22]=spread * vec2(0.0,2.0);
      tc_offset[23]=spread * vec2(1.0,2.0);
      tc_offset[24]=spread * vec2(2.0,2.0);


      vec4 sample[25];

      for (int i=0 ; i<25 ; i++)
      {
        sample[i]=texture2D(myTex , tc + tc_offset[i]);
      }

      vec4 blurredColor=(
                        (1.0 * (sample[0] + sample[4] + sample[20] + sample[24])) +

                        (4.0 * (sample[1] + sample[3] + sample[5] + sample[9] +
                               sample[15] + sample[19] + sample[21] + sample[23])) +

                        (7.0 * (sample[2] + sample[10] + sample[14] + sample[22])) +

                        (16.0 * (sample[6] + sample[8] + sample[16] + sample[18])) +

                        (26.0 * (sample[7] + sample[11] + sample[13] + sample[17])) +
                        (41.0 * sample[12])
                        )/ 273.0;
      //blurredColor.a=1.0;
      return(blurredColor);
}


vec2 shadowBlur3(sampler2D myTex, vec2 tc){

      float sample[9];

      float spread=1.0/screenX;//  * unpackToFloat(texture2D(myTex , tc).rg)*farClip/16.0;
      //float spread=0.250/shadow_size;//   * texture2D(myTex , tc).a/32.0;

      tc_offset[0]=spread * vec2(-1.0,-1.0);
      tc_offset[1]=spread * vec2(0.0,-1.0);
      tc_offset[2]=spread * vec2(1.0,-1.0);

      tc_offset[3]=spread * vec2(-1.0,0.0);
      tc_offset[4]=spread * vec2(0.0,0.0);
      tc_offset[5]=spread * vec2(1.0,0.0);

      tc_offset[6]=spread * vec2(-1.0,-1.0);
      tc_offset[7]=spread * vec2(0.0,-1.0);
      tc_offset[8]=spread * vec2(1.0,-1.0);

      for (int i=0 ; i<9 ; i++)
      {
        sample[i]=unpackToFloat(texture2D(myTex , tc + tc_offset[i]).rg);
      }

      float blurredZ=(
                         sample[0] + (2.0* sample[1]) + sample[2] +
                         (2.0*sample[3]) + sample[4] + (2.0*sample[5]) +
                         sample[6] + (2.0* sample[7]) + sample[8]
                        )/ 13.0;

      vec2 result=packToVec2(blurredZ);

      return(result);
}



void main(){


      for (int i=0;i<25;i++)
            tc_offset[i]=vec2(0.0,0.0);


    gl_FragColor=texture2D(tex,texCoord,0.0);
    //gl_FragColor.rg=shadowBlur3(tex,texCoord);
    //gl_FragColor.rg=blur3(tex,texCoord).rg * -100.0;

}
