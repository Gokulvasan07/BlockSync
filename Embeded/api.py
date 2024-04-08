import requests
import json
reqUrl = "https://iot-project-2f20e-default-rtdb.firebaseio.com/Appliances.json?auth=DCJqdn05xseUfXIve8SzxWu0wMEpkdtn1gE2WzRG"

headersList = {
    "Accept": "*/*",
    "User-Agent": "Thunder Client (https://www.thunderclient.com)"
}

payload = ""

response = requests.request("GET", reqUrl, data=payload,  headers=headersList)
aList = response.json()

print(aList['temperature'])
