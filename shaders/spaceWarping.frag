uniform vec2 u_resolution;
uniform vec2 xy;
uniform bool neg;
void main(){
    float dist = 0.0;
    dist = distance(gl_FragCoord.xy / u_resolution.y,xy/ u_resolution.y);
    if(dist == 0){
        dist += 0.01;
    }
    if(neg){
    gl_FragColor = vec4(255,0,0,0.01/dist);
    }else{
        gl_FragColor = vec4(0,0,255, 0.01 / dist);
    }
}