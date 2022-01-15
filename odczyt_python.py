import numpy as np
from time import sleep
import serial #pip install pyserial
import time
import json
import matplotlib.pyplot as plt
import keyboard #pip install keyboard

"""Ustaw odpowiedni COM"""
plt.ion()   #newly created figures will be shown immediately; figures will automatically redraw on change;

hSerial = serial.Serial('COM10', 115200, timeout=1, parity=serial.PARITY_NONE)  #Ustawienie portu szeregowego

#Wysylanie waidomosci do kontrolera
hSerial.write(b'printOn;')  #Write the bytes data to the port.
sleep(0.5)
set_point = 150;
hSerial.write(b'setValue=150;')
sleep(0.5)
# hSerial.write(b'freq=1;')
# sleep(0.5)

#timestr = time.strftime("%Y%m%d-%H%M%S")    #data
#hFile = open("pomiary_python%s.txt" % (timestr), "a")            #otwarcie pliku z temperaturami, 'a' - otwarcie pliku do konfiguracji
hFile = open("pomiary_python.txt", "a")

hSerial.reset_input_buffer()
hSerial.flush()                 #chyba wywalenie tego
temperature_samples = [];
t = [];
t_value=0;
plt.show()
#Program pobiera pomiar z czujnika, zamienia to na format json, printuje ta wartosc w pythonie jak i zapisuje ja do pliku.
#nastepnie dodaje ta wartosc do wektora i plotuje caly wykres, dodaje do niego ta nowa wartosc
while True:
    text = hSerial.readline()           #Pobranie 1 linijki pomiaru
    temperature = 0
    sample = 0
    try:
        sample = json.loads(text)               #pobranie tekstu w formie json
        temperature = sample["lux"]     #
    except ValueError:                          #jezeli sie nie uda wczytac JSON
        print("Bad JSON")
        print("%s\r\n" % {text})
        hSerial.flush()
        hSerial.reset_input_buffer()
    print(temperature)                          #printuje temperature w pythonie
    hFile.write("%.2f," % temperature)          #zapisanie zczytanej temperatury do pliku
    temperature_samples.append(temperature);    #dodanie pomierzonej wartosci do wektora temperatur
    t.append(t_value);
    t_value = t_value + 1
    # Plot results
    plt.clf()                                   #czyszczenie wykresu
    plt.plot(t,temperature_samples, '.', markersize=5);             #plotowanie wykresu temperatury
    plt.title("BMP1750 STM32 (controller sp=%d Lux)." % set_point)
    plt.xlabel("Time (s)")
    plt.ylabel("Light [lux]")
    #plt.show()
    plt.pause(0.001)
    if keyboard.is_pressed("q"):
        break  # finishing the loop
hSerial.close()
hFile.close()
