import sensor, image, time, lcd
from pyb import UART

'''
output_str_red = ""
output_str_green = ""
output_str_blue = ""
'''

red_threshold  = (30, 60, 20, 60, -20, 40)#(0, 100, 19, 77, -9, 127)
green_threshold  = (30, 66, -50, 0, 0, 61)
blue_threshold  = (35, 60, -16, 33, -60, -10)#(0, 36, -41, 22, -45, 39)#(27, 88, -29, 12, -43, -15)
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQVGA)
#sensor.set_windowing((400,400))
sensor.set_auto_gain(True)
sensor.set_auto_whitebal(True)
#sensor.skip_frames(10)
clock = time.clock()
uart = UART(3, 115200) #P4 Tx, P5 Rx--PD2 Rx, PC12 Tx
lcd.init()

flag = '1'
#roi = (40, 40, 80, 80)

def find_max(blobs):
    max_size=0
    for blob in blobs:
        if blob.pixels() > max_size:
            max_blob = blob
            max_size = blob.pixels()
    return max_blob

#while(True):
    ##if uart.any():
        ##command = uart.read().decode()
        ##if command == 'star':
            #while(True):
                #img = sensor.snapshot()
                #blobs_red = img.find_blobs([red_threshold], roi = (40, 30, 80, 60), pixels_threshold=3000, area_threshold=4000)
                #blobs_green = img.find_blobs([green_threshold], roi = (30, 40, 80, 60), pixels_threshold=3000, area_threshold=4000)
                #blobs_blue = img.find_blobs([blue_threshold], roi = (40, 30, 80, 60), pixels_threshold=3000, area_threshold=4000)
                #if blobs_red:
                    #max_blob_red=find_max(blobs_red)
                    #img.draw_rectangle(max_blob_red.rect(), color = (255, 0, 0))
                    #img.draw_cross(max_blob_red.cx(), max_blob_red.cy())
                    #lcd.display(img)
                    #output_str_red = "1"
                    ##if max_blob_red.cx() == 64:
                    #uart.write("@" + output_str_red + "%" + "A")
                    #print(output_str_red)
                #elif blobs_green:
                    #max_blob_green=find_max(blobs_green)
                    #img.draw_rectangle(max_blob_green.rect(), color = (0, 255, 0))
                    #img.draw_cross(max_blob_green.cx(), max_blob_green.cy())
                    #lcd.display(img)
                    #output_str_green = "2"
                    ##if max_blob_green.cx() == 64:
                    #uart.write("@" + output_str_green + "%" + "A")
                    #print(output_str_green)
                #elif blobs_blue:
                    #max_blob_blue=find_max(blobs_blue)
                    #img.draw_rectangle(max_blob_blue.rect(), color = (0, 0, 255))
                    #img.draw_cross(max_blob_blue.cx(), max_blob_blue.cy())
                    #lcd.display(img)
                    #output_str_blue = "3"
                    ##if max_blob_blue.cx() == 64:
                    #uart.write("@" + output_str_blue + "%" + "A")
                    #print(output_str_blue)
                #else:
                    #uart.write("@" + "0" + "%" + "A")
                    #print("0")
                    #lcd.display(img)
                #if uart.any():
                    #command = uart.read().decode()
                    #if command == 'stop':
                        #break
      # print(clock.fps())
while(True):
#if uart.any():
    #command = uart.read().decode()
    #if command == 'star':
        while(True):
            img = sensor.snapshot()
            blobs_red = img.find_blobs([red_threshold], roi = (60, 30, 50, 60), pixels_threshold=2800, area_threshold=3000)
            blobs_green = img.find_blobs([green_threshold], roi = (30, 40, 80, 60), pixels_threshold=3500, area_threshold=4000)
            blobs_blue = img.find_blobs([blue_threshold], roi = (40, 30, 80, 60), pixels_threshold=3500, area_threshold=4000)
            if blobs_red:
                max_blob_red=find_max(blobs_red)
                img.draw_rectangle(max_blob_red.rect(), color = (255, 0, 0))
                img.draw_cross(max_blob_red.cx(), max_blob_red.cy())
                lcd.display(img)
                output_str = "red"
                #if max_blob_red.cx() == 64:
                #uart.write("@" + output_str_red + "%" + "A")
                #print(output_str_red)
            elif blobs_green:
                max_blob_green=find_max(blobs_green)
                img.draw_rectangle(max_blob_green.rect(), color = (0, 255, 0))
                img.draw_cross(max_blob_green.cx(), max_blob_green.cy())
                lcd.display(img)
                output_str = "green"
                #if max_blob_green.cx() == 64:
                #uart.write("@" + output_str_green + "%" + "A")
                #print(output_str_green)
            elif blobs_blue:
                max_blob_blue=find_max(blobs_blue)
                img.draw_rectangle(max_blob_blue.rect(), color = (0, 0, 255))
                img.draw_cross(max_blob_blue.cx(), max_blob_blue.cy())
                lcd.display(img)
                output_str = "blue"
                #if max_blob_blue.cx() == 64:
                #uart.write("@" + output_str_blue + "%" + "A")
                #print(output_str_blue)
            else:
                output_str = "none"
                #uart.write("@" + "0" + "%" + "A")
                #print("0")
                lcd.display(img)

            if (flag == '1' and output_str != "none"):
                target = output_str
                flag = '2'
            elif (flag == '2' and output_str == "none"):    #离开目标色块
                flag = '3'
            elif (flag == '3' ):     #寻找匹配色块
                colour = output_str
                if (colour == target):
                    flag = '4'      #找到匹配色块
            elif (flag == '4' and output_str == "none"):
                flag = '5'
                uart.write("@" + '4' + "%" + "A")
                print('4')
