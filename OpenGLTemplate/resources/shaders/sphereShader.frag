#version 330 core

in vec3 vColour;			// Note: colour is smoothly interpolated (default)
in float fIntensity;
uniform int levels;
out vec4 vOutputColour;

void main()
{	
  vec3 quantisedColour = floor (vColour *levels) / levels;
   float colourVal;
   if (fIntensity > 0.75) {
   vOutputColour = vec4 ( 1.0,0.0,0.0,1.0);
   } else if (fIntensity > 0.5) {
   vOutputColour = vec4 ( 0.6,0.0,0.0,0.5);
   } else {
   vOutputColour = vec4 ( 0.3,0.0,0.0,0.2);
     
   }
   vOutputColour = vec4 (quantisedColour, 1.0);

}
