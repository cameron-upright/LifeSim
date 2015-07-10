!!ARBfp1.0

ATTRIB iNormal     = fragment.texcoord[1];
ATTRIB iLightDir   = fragment.texcoord[2];
ATTRIB iHalfVector = fragment.texcoord[3]; 

PARAM ambientColor  = state.lightprod[0].ambient;
PARAM diffuseColor  = state.lightprod[0].diffuse;
PARAM specularColor = state.lightprod[0].specular;

TEMP dots, color, temp, magnitude, lightDir, halfVector;

# Normalize light direction
DP3 magnitude, iLightDir, iLightDir;
RSQ temp, magnitude.x;
MUL lightDir, iLightDir, temp;

# Normalize half vector
DP3 magnitude, iHalfVector, iHalfVector;
RSQ temp, magnitude.x;
MUL halfVector, iHalfVector, temp;

# Calculate lighting
DP3 dots.x, iNormal, lightDir;
DP3 dots.y, iNormal, halfVector;
MOV dots.w, state.material.shininess.x;
LIT dots, dots;

# Accumulate color
MAD color, dots.yyyy, diffuseColor, ambientColor;
MAD result.color, dots.zzzz, specularColor, color;
MOV result.color.w, diffuseColor.w;

END
