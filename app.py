from flask import Flask, request
import requests
from config import endPoint

def call_api(id):
    request_body = {"id":id}
    res = requests.post(endPoint, json=request_body)
    return res.text
app = Flask(__name__)
@app.route('/',methods=('GET','POST'))
def index():
    return "hello world"

@app.route('/buttonCount',methods=('GET','POST'))
def buttonCount():
    if request.method == 'POST':
        inputRequest = request.get_json(force=True)
        id = inputRequest['id']
        
        return call_api(id)
if __name__ == '__main__':
    app.run(debug=True, host="0.0.0.0")
