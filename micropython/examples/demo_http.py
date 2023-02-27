import time

try:
    import ppwhttp
except ImportError:
    raise RuntimeError("Cannot find ppwhttp. Have you copied ppwhttp.py to your Pico?")


ppw = ppwhttp.PPWHTTP()

# Edit your routes here
# Nothing fancy is supported, just plain ol' URLs and GET/POST methods
@ppw.route("/", methods=["GET", "POST"])
def get_home(method, url, data=None):
    if method == "POST":
        pass

    return """<p>Demo site</p>""".format()


@ppw.route("/test", methods="GET")
def get_test(method, url):
    return "Hello World!"


ppw.start_wifi()

server_sock = ppw.start_server()
while True:
    ppw.handle_http_request(server_sock)
    time.sleep(0.01)


# Whoa there! Did you know you could run the server polling loop
# on Pico's *other* core!? Here's how:
#
# import _thread
#
# def server_loop_forever():
#    # Start a server and continuously poll for HTTP requests
#    server_sock = ppw.start_server()
#    while True:
#        ppw.handle_http_request(server_sock)
#        time.sleep(0.01)
#
# Handle the server polling loop on the other core!
# _thread.start_new_thread(server_loop_forever, ())
#
# # Your very own main loop for fun and profit!
# while True:
#     print("Looper")
#     time.sleep(5.0)
