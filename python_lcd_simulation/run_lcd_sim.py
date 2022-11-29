import time
import zmq
import lcd_sim
import logging
import threading

def thread_function(lcdScreen):
    logging.info("Thread starting")
    lcdScreen.start_game()
    logging.info("Thread finishing")

if __name__ == "__main__":
    format = "%(asctime)s: %(message)s"
    logging.basicConfig(format=format, level=logging.INFO,
                        datefmt="%H:%M:%S")

    #logging.info("Main    : before creating thread")

    context = zmq.Context()
    socket = context.socket(zmq.REP)
    socket.bind("tcp://*:5555")

    config_message = socket.recv()
    #  Send reply back to client
    socket.send(b"World")

    #print("Received request: %s" % config_message)

    rx_lcd_width, rx_lcd_height = [int(i) for i in config_message.split(b",")]

    #print(rx_lcd_width, rx_lcd_height)
    #exit(1)

    lcd_screen = lcd_sim.LcdScreen(rx_lcd_width, rx_lcd_height)
    lcdScreenThread = threading.Thread(target=thread_function, args=(lcd_screen,))
    logging.info("Main    : before running thread")

    lcdScreenThread.start()


    while(1):
        
        if lcd_screen.isRunning() == False:
            socket.close()
            break

            #  Wait for next request from client
        message = socket.recv()
        #print("Received request: %s" % message)
        #print(len (message))
        #  Do some 'work'
        #time.sleep(1)

        if message.startswith(b"end"):
            lcd_screen.terminate()
            socket.close()
            break

        lcd_screen.setBuffer(message)
        #lcd_screen.setPixel(x, y, value)

        #  Send reply back to client
        socket.send(b"World")

        

    lcdScreenThread.join()
    logging.info("Main    : all done")
    exit(1)
