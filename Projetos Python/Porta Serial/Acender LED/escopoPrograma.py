
import serial
import time

arduino = serial.Serial('/dev/tty.usbmodem1411', 9600)
"""
    Métodos ========================================
"""
def ligarDesligar():
    #Loop infinito para ficar recebendo mensagens
    #conseguir checar se chegou uma mensagem e imprimir ela  
    while 1:
        #Um IF caso a mensagem seja "123", para imprimir alguma coisa
        data = arduino.readline()[:-2]
        if data:
            print("Chegou a mensagem")
            print(data)
            arduino.write('O')

            if data == "123":
                print("Entrou no if 123 com o numero")
                print(data)
            time.sleep(1)
        else: 
            print("Não recebeu")
            time.sleep(1)
   
    
    #e enviar uma mensagem de volta para o arduino
"""
    Main =============================================
"""
time.sleep(2) #Esperar a inicialização
ligarDesligar()