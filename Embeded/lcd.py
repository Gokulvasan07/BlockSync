import json
import requests
from signal import signal, SIGTERM, SIGHUP, pause
from time import sleep
from rpi_lcd import LCD
lcd = LCD()


def api():
    reqUrl = "https://iot-project-2f20e-default-rtdb.firebaseio.com/Appliances.json?auth=DCJqdn05xseUfXIve8SzxWu0wMEpkdtn1gE2WzRG"

    headersList = {
        "Accept": "*/*",
        "User-Agent": "Thunder Client (https://www.thunderclient.com)"
    }

    payload = ""

    response = requests.request(
        "GET", reqUrl, data=payload,  headers=headersList)
    aList = response.json()
    return aList

# print(aList['temperature'])


def safe_exit(signum, frame):
    exit(1)


while True:
    try:
        signal(SIGTERM, safe_exit)
        signal(SIGHUP, safe_exit)
        aList = api()
        lcd.text("Ethic Electronics", 1)
        lcd.text("Air Purity : 76%", 2)
        sleep(5)
        lcd.text("Temperature:"+str(aList['temperature'])+"C", 1)
        lcd.text("Humidity :"+str(aList['humidity']), 2)
        sleep(5)
        lcd.text("Ethic Electronics", 1)
        lcd.text("Air Purity : 76%", 2)
        sleep(5)
        lcd.text("Temperature:"+str(aList['temperature'])+"C", 1)
        lcd.text("Humidity :"+str(aList['humidity']), 2)
        sleep(5)
    except KeyboardInterrupt:
        pass
    finally:
        lcd.clear()


# "appliance1": "1",
# "appliance2": "1",
# "appliance3": "1",
# "appliance4": "1",
