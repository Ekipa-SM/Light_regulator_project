import numpy as np
from time import sleep
import serial #pip install pyserial
import time
import json
import matplotlib.pyplot as plt
import keyboard #pip install keyboard

"""Ustaw odpowiedni COM"""
plt.ion()   #newly created figures will be shown immediately; figures will automatically redraw on change;
hSerial = serial.Serial('COM3', 38400, timeout=1, parity=serial.PARITY_NONE)  #Ustawienie portu szeregowego

#Wysylanie waidomosci do kontrolera
hSerial.write(b'printOn;')  #Write the bytes data to the port.
sleep(0.5)
set_point = 150;
hSerial.write(b'setValue=200;')
sleep(0.5)
hSerial.write(b'freq=2;')
sleep(0.5)

hFile = open("pomiary_python.txt", "a")

hSerial.reset_input_buffer()
hSerial.flush()                 #chyba wywalenie tego
light_samples = [];
pid_samples = [];
setValue_samples = [];
t = [];
t_value=0;
plt.show()
#Program pobiera pomiar z czujnika, zamienia to na format json, printuje ta wartosc w pythonie jak i zapisuje ja do pliku.
#nastepnie dodaje ta wartosc do wektora i plotuje caly wykres, dodaje do niego ta nowa wartosc
while True:
    text = hSerial.readline()           #Pobranie 1 linijki pomiaru
    light = 0
    pid = 0
    setValue = 0
    sample = 0
    try:
        sample = json.loads(text)               #pobranie tekstu w formie json
        light = sample["lux"]     #
        pid = sample["pid"]
        setValue = sample["set"]
    except ValueError:                          #jezeli sie nie uda wczytac JSON
        print("Bad JSON")
        print("%s\r\n" % {text})
        hSerial.flush()
        hSerial.reset_input_buffer()
    print(light)                          #printuje temperature w pythonie
    hFile.write("%.2f," % light)          #zapisanie zczytanej temperatury do pliku
    light_samples.append(light);    #dodanie pomierzonej wartosci do wektora temperatur
    pid_samples.append(pid);
    setValue_samples.append(setValue);
    t.append(t_value);
    t_value = t_value + 1
    # Plot results
    plt.clf()                                   #czyszczenie wykresu
    plt.plot(t, light_samples, '.', label='sensivity', markersize=5);             #plotowanie wykresu temperatury
    plt.plot(t, pid_samples, '.', label='pid output', markersize=5);
    plt.plot(t, setValue_samples, '.', label='set value', markersize=5);
    plt.title("BMP1750 STM32 (controller sp=%d Lux)." % set_point)
    plt.xlabel("Time (s)")
    plt.ylabel("Light [lux]")
    plt.legend()
    #plt.show()
    plt.pause(0.001)
    if keyboard.is_pressed("q"):
        break  # finishing the loop
hSerial.close()
hFile.close()
