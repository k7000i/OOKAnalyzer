import socket, time


if __name__ == '__main__':

    success = False
    while not success:
        try:
            s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            s.connect(('127.0.0.1', 10429))
            s.send(b'LOAD_FROM_FILE, up_down_stop_dc104_433mhz.logicdata\0')
            success = True
            s.close()
            exit()
        except:
            pass
        time.sleep(3)