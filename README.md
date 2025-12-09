# multiFileSystem
Un programa base para un sistema de almacenamiento dual  
* root . / Internal /.. /.. /.. 
* toor . / External /.. /.. /.. 
  
###### <br> "a hombros de gigantes"  
###### <br>"la informacion que no es compartida en tiempo y forma, simplemente muere."  
###### <br>  

Un programa base para un sistema de almacenamiento dual,  
co-existiendo sincronizadamente LittleFS + extSDcard,  
como doble posible origen de datos.  
    
Carga la configuracion desde LittleFS,  
Si no existe, la busca en extSDcard,  
Si no existe en ninguna parte..  
Genera un archivo de config.txt por defecto en formato json.  

___________________________________________________________________  
Diagrama de conexión:  
Arduino NodeMCU amica 1.0 v3 ESP8266  
  VCC --------> +3.3vcc ---> SD card <---|  
  GND ---------> GND ----> SD card <---|  
  gpio14 D5 ---> SCLK ----> SD card <---|  
  gpio12 D6 ---> MISO ---> SD card <---|  
  gpio13 D7 ---> MOSI ---> SD card <---|  
  gpio15 D8 ---> CC/SS ---> SD card <---|  
___________________________________________________________________  
_Son bienvenidos los Aportes, Colaboraciones, y Consejos._  
_GRACIAS._  
___________________________________________________________________  
  + __Trabajando :__  
    - [ ] Reservado. (evaluando posibles cambios y mejoras necesarias) [V-0.0.2]()    
    - [x] Trabajando en Sistema Dual. [V-0.0.1]()  
    - [x] Trabajando en Sistema Dual. [V-0.0.0]()  
___________________________________________________________________  
