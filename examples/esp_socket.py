import time

SOCK_STREAM = 1
TCP_MODE = 0

def getaddrinfo(host, port, nan, sss):
    return [(2, 1, 0, '', ('88.99.97.200', 443))]


class socket:
    def __init__(self, aa, bb, cc, picowireless):
        self.picowireless = picowireless
        self.client = 0
        self.timeout=5000
        self.res = b''
    
    def settimeout(self, sts):
        self.timeout=sts

    def connect(self, aa):
        self.picowireless.client_start((88, 99, 97, 200), 80, self.client, TCP_MODE)

        t_start = time.time()
        self.timeout /= 1000.0

        while time.time() - t_start < self.timeout:
            state = self.picowireless.get_client_state(self.client)
            if state == 4:
                return True
            time.sleep(1.0)

        return False
    
    def write(self, dat):
        self.picowireless.send_data(self.client, dat)
        
    def readline(self):
        if (self.res == b''):
            self.__read()
            
        tmp=self.res.decode('utf-8').split("\r\n")
        resval=tmp.pop(0).encode('utf-8') # Pops and teruns first value.
        self.res = "\r\n".join(tmp).encode('utf-8')
            
        return resval
    
    def read(self, dd):
        if (self.res == b''):
            self.__read()
            
        return self.res
        
    def __read(self):
        t_start = time.time()
        while True:
            if time.time() - t_start > self.timeout:
                self.picowireless.client_stop(self.client)
                print("HTTP request timed out...")
                return False

            avail_length = self.picowireless.avail_data(self.client)
            if avail_length > 0:
                break

        print("Got response: {} bytes".format(avail_length))

        response = b""

        while len(response) < avail_length:
            data = self.picowireless.get_data_buf(self.client)
            response += data

        #response = response.decode("utf-8")
        self.res=response
        
    def close(self):
        self.picowireless.client_stop(self.client)


