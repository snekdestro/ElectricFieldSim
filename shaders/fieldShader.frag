


uniform vec2 u_resolution;
uniform vec2 xy;
uniform float waveTime;




void main(){
    
    float dist = 0.0;
    
    //dist = distance(gl_FragCoord.xy / u_resolution, vec2(0.5,0.5));
    dist = distance(gl_FragCoord.xy / u_resolution.y, xy / u_resolution.y);
    float a = abs(sin(waveTime));
    float b = abs(cos(waveTime));
    bool dosomething = false;
    
    float c= waveTime;
    
        dosomething = false;

  
        for(int i = -15; i < 9; i++){
            float add = 0.2 * i;
            float wi = waveTime + add;
            if(wi <= dist && dist <= wi + 0.05){
                dosomething = true;
                break;
                }
            }
       
    


    if(dosomething){
        gl_FragColor = vec4(a,0.0,b, 1.0/(dist*10));
    }else{
        gl_FragColor = vec4(0,0,0,0);
    }
    
}