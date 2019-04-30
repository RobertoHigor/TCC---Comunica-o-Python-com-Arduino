import serial
import time

from UsuarioDao import UsuarioDao
from Usuario import Usuario

arduino = serial.Serial('COM4', 9600)

"""
    Métodos=================================
"""
#Inserir na tabela LOG quando a porta foi aberta
def inserirRegistro(result):
    usr = UsuarioDao()     
    usuarioRetorno = Usuario(result[0], result[1], result[2])               
    usr.inserirLog(usuarioRetorno)

#Método para chegar se achou um registro com a senha digitada
def logar(senha):
    usr = UsuarioDao()
    result = usr.logarUsuario(senha)

    if result is not None:        
        arduino.write('S'.encode())
        print("Sucesso")    
        #Inserir no registro o usuário que possui a senha digitada    
        inserirRegistro(result)
         
    else:
        #print("falha: {}".format(result[0]))
        arduino.write('F'.encode())
        print("Falha")
        

"""
    Main ====================================
"""
#Instanciando as classes (teste senha 15158)
        #usuario1 = Usuario('Zuko Iroh', 'zukofire', senha)

        #Criar o banco de dados e inserir usuário
        #usr.criarBanco()
        #usr.inserirUsuario(usuario1)
while(1):
    senha = int(arduino.readline().decode("UTF-8")[:-2])
    if senha:
        print(senha)       
        logar(senha)        
