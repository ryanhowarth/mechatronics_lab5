#!/usr/bin/python
import serial
import datetime
import time
import matplotlib.pyplot as plt
import serial.tools.list_ports

#Change these parameters to match your environment.
path = '/dev/ttyACM0'
#!Change the path according to ls /dev

folder = ''



baud = 9600




class import_serial_data():

    ser = []
    data_file = []
    packet_size = 3
    file_name = ''
    start_time = time.time()
    last_time = time.time()
    arduino_id = '2341:43'
    def __init__(self, port='/dev/ttyACM0', baud=115200, folder='unknown_folder'):
        #Connecting to Serial Port.
	#port = self.find_com_port()
        print 'Com Port:' + port
	self.ser = serial.Serial(port, baud)
        self.ser.flushInput()
        self.create_file(folder)
        self.setup_plotting()

        try:
            self.main_run_loop()
        finally:
            self.close_file()


    def find_com_port(self):
        port_list  = list(serial.tools.list_ports.comports())
        for port in port_list:
            for i in port:
                if self.arduino_id in i:
                    return port[0]
        return 'Arduino com port not Found.'


    def setup_plotting(self):
        plt.ion()
        plt.axis([0, 50, 0, 1024])
        plt.xlabel('Time (s)')
        plt.ylabel('Raw Arduino ADC Values (0-1024)')
        plt.title('Real Time Plot of Raw Load Cell Data')
        plt.pause(0.0001)
        self.start_time = time.time()


    def create_file(self, folder):
        #Setting up file name.
        date_time_string = str(datetime.datetime.now())[:-7]
        date_time_string = date_time_string.replace('-', '_')
        date_time_string = date_time_string.replace(':', '_')
        description = raw_input("Please enter a description of data being collected:\n")
        self.file_name = folder + description + '_' + date_time_string + '.txt'
        print(self.file_name)

        #Creating the file.
        self.data_file = open(self.file_name, 'w')
        self.data_file.write("SBD16 Raw Force Data\n")
        self.data_file.write(description + '\n')
        self.data_file.write(date_time_string + '\n')
        self.data_file.write('\n')

    #@profile
    def process_data(self, val):
        d = [ord(i) for i in val]
        #print('val: ', d)
        if d[2] == 255:
            num =  d[0]*256 + d[1]
            self.data_file.write(str(num))
            self.data_file.write('\n')
            self.plot(num)
            #print('gets here')
        elif d[1] == 255:
            self.ser.read(2)
        elif d[0] == 255:
            self.ser.read(1)
    #@profile
    def plot(self, val):
        if (time.time() - self.last_time > .5):
            plt.plot((time.time()-self.start_time), val, 'xb-')
            #plt.draw()
            plt.pause(0.0001)
            self.last_time = time.time()

    #@profile
    def main_run_loop(self):
        while True:
            if self.ser.inWaiting():
                val = self.ser.read(self.packet_size)
                self.process_data(val)

    #Closing file properly.
    def close_file(self):
        print ' Data written to: ', self.file_name, '\n'
        self.data_file.close()

x = import_serial_data(path, baud, folder)
