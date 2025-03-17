uniform int size;
uniform float data[140];
uniform float signs[70];




vec3 deg2RGB(float theta){
    float kr = mod(5 + theta * 6.0, 6.0);
    float kg = mod(3 + theta * 6.0, 6.0);
    float kb = mod(1 + theta * 6.0, 6.0);
    float r = 1 - max(min(kr,min(4.0-kr,1.0)),0.0);
    float g = 1 - max(min(kg,min(4.0-kg,1.0)),0.0);
    float b = 1 - max(min(kb,min(4.0-kb,1.0)),0.0);
    return vec3(r,g,b);
}
void main(){
    vec2 uv = vec2(gl_FragCoord.x, gl_FragCoord.y);
    vec2 force = vec2(0,0);
    for(int i = 0; i < size; i+=2) {
        vec2 xy = vec2(data[i],data[i+1]);
        vec2 ab = xy - uv;
        float dist = dot(ab, ab);
        if(dist == 0){
            continue;
        }
        vec2  f = vec2(1000000) / dist;
        force += f * signs[i/2] * normalize(ab);
        
    }
    if(length(force) == 0){
        gl_FragColor =(vec4(0,0,0,0));
        return;
    }
    float fr = length(force);
    
    for(int i = 1; i < 12; i++){
        if(5.0  * i< fr && fr < 5.2 * i){
        gl_FragColor = vec4(0.0,0.0,0.0,1.0);
        return;
    }

    }
    
    force = normalize(force);
    float theta = atan(force.y / force.x);
    
    if(force.x < 0 && force.y > 0){
        theta += 3.14159265358979;
    }
    if(force.y < 0 && force.x < 0){
        theta += 3.141590265358979;
    }
    
    if(theta < 0){
        theta  += 2 *3.14159265358979;
    }
    
    vec3 rgb = deg2RGB(theta / 3.14159265358979 /2.0);
    gl_FragColor = vec4(rgb,1.0 - 1.0 /(fr));
    
}