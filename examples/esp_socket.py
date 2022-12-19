from picowireless import PicoWireless
import time

SOCK_STREAM = 1

class SOCKET_MODE():
    TCP = 0
    UDP = 1
    TLS = 2

def getaddrinfo(picowireless, host: str, port, _x, _y):
    if (picowireless.req_host_by_name(host)):
        ip=picowireless.get_host_by_name()
        return [(2, 1, 0, '', (ip, port))]
    
    return [None]


class socket:
    def __init__(self, _x, _y, _z, picowireless: PicoWireless):
        # TODO: make use of client number from esp
        self.picowireless = picowireless
        self.client=picowireless.get_socket()
        self.timeout=15
        self.res = b''
    
    def settimeout(self, timeout):
        self.timeout=timeout

    def connect(self, host: tuple, mode = SOCKET_MODE.TCP):
        url=host[0]
        port=host[1]
        print(url)
        if (port == 443):
            self.picowireless.client_start((0,0,0,0), port, self.client, mode, url)
        else:
            self.picowireless.client_start(url, port, self.client, mode)

        t_start = time.time()

        while time.time() - t_start < self.timeout:
            state = self.picowireless.get_client_state(self.client)
            if state == 4:
                return True
            time.sleep(1.0)
            
        print("Connection failed")

        return False
    
    def write(self, dat: str|bytes):
        print(dat)
        self.picowireless.send_data(self.client, dat)
        
    def readline(self):
        if (len(self.res) == 0):
            self.res = self.__read(0)
        else:
            # Check if data is missing
            avail_length = self.picowireless.avail_data(self.client)
            if (avail_length > 0):
                self.res += self.__read(avail_length)
            
        tmp=self.res.decode('utf-8').split("\r\n")
        resval=tmp.pop(0).encode('utf-8') # Pops and teruns first value.
        self.res = "\r\n".join(tmp).encode('utf-8')
            
        return resval
    
    def read(self, count=0) -> bytes|str:
        # TODO: Maby add count to read or something. Or maybe just remove it entirely.
        if (len(self.res) == 0):
            self.res = self.__read(count)

        # If the buffer is smaller than the expected data, then try and get more.
        if (len(self.res) < count):
            avail_length = self.picowireless.avail_data(self.client)
            if (avail_length > 0):
                self.res += self.__read(avail_length)
            
        if (count == 0):
            return self.res
        else:
            resVal = self.res[:count]
            self.res = self.res[count:]
            return resVal
        
    def __read(self, count) -> bytes:
        t_start = time.time()
        print(self.timeout)
        while True:
            if time.time() - t_start > self.timeout:
                self.picowireless.client_stop(self.client)
                print("HTTP request timed out...")
                return b''

            avail_length = self.picowireless.avail_data(self.client)
            if (avail_length > 0 or count):
                break

        print("Got response: {} bytes".format(avail_length))

        response = b""

        # If requested more than detected, then just try and read what is requested.
        if (count > avail_length):
            avail_length = count
        while len(response) < avail_length:
            data = self.picowireless.get_data_buf(self.client)
            # If no data is left, then just stop getting.
            if (data is None or len(data) == 0):
                break
            response += data

        #response = response.decode("utf-8")
        return response
        
    def close(self):
        self.picowireless.client_stop(self.client)
