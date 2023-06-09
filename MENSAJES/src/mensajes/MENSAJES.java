package mensajes;
//
//import gnu.io.CommPort;
//import gnu.io.CommPortIdentifier;
//import gnu.io.SerialPort;
//import java.io.PrintStream;
//import java.util.Enumeration;
//import java.util.Scanner;

public class MENSAJES {
    
    public static void main(String[] args) throws Exception {
       
//        Enumeration Puertos = CommPortIdentifier.getPortIdentifiers();
//        CommPortIdentifier Mipuerto = null;
//        Scanner Mensaje;
//        PrintStream Contestacion;
//        
//        while(Puertos.hasMoreElements()){
//            Mipuerto = (CommPortIdentifier) Puertos.nextElement();
//            if(Mipuerto.getName().equals("COM7")){
//                break;
//            }
//        }
//        
//        CommPort Puerto = Mipuerto.open("UART", 2000);
//        SerialPort MiSP = (SerialPort) Puerto;
//        MiSP.setSerialPortParams(19200, SerialPort.DATABITS_8, SerialPort.STOPBITS_1, SerialPort.PARITY_NONE);
//        
//        Mensaje = new Scanner(MiSP.getInputStream());// Flujo de entrada
//        Contestacion = new PrintStream(MiSP.getOutputStream());// Flujo de salida
//       
//        while(Mensaje.hasNextInt()){
//            Mensaje.close();
//            Mensaje = null;
//            Mensaje = new Scanner(MiSP.getInputStream());
//        
//        }
//        
//        int prueba = Mensaje.nextInt();
//        Contestacion.println("PRUEBA");
    }   
}
