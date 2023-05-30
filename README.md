# Button Counter Project

The goal of this project is to track the number of times a button connected to an arduino is pushed. The button pushes are recorded in a DynamoDB table, allowing you to keep track of the count over time, and allow the data to persist even after the arduino is powered off

What's Here
-----------
* README.md - this file
* lambda_function.py - Code for the lambda function that will handle requests
* buttonCountArduinoCode.ino - Arduino code for connecting to api. Be sure to update server if you intend on running this
* app.py - If you have trouble connecting the arduino to the API gateway directly like i did you can use make requests to this flask app and it will forward them to the API
## Requirements

- Arduino board with WiFi capability (e.g., Arduino Uno WiFi Rev2, Arduino MKR WiFi 1010)
- Button connected to the Arduino board
- LED (optional)
- AWS account with access to DynamoDB and API Gateway

## Setup

1. Connect the button to the Arduino board according to the information in buttonCountArduinoCode.ino
2. Install the necessary libraries: WiFiNINA and SPI. These libraries enable WiFi connectivity on the Arduino board.
3. Create a DynamoDB table to store the button count data. Make sure to note the table name for later use.
4. In AWS Lambda create a Lambda function for python and copy the code from lambda_function.py
5. Set up an API Gateway with a POST method to receive button push events and integrate it with the Lambda function.
6. Deploy the API Gateway and note the endpoint URL.
7. Update the Arduino code with the API Gateway endpoint URL (or use the flask app as an intermediary)
8. Upload the Arduino code to the board and ensure it is connected to the WiFi network.

## Usage

1. Power on the Arduino board.
2. Each time the button is pushed, the Arduino will send a POST request to the API Gateway endpoint.
3. The API Gateway will trigger the Lambda function, which will update the DynamoDB table with the button count.
4. The current button count will be sent back by the API

## Troubleshooting

- If the Arduino fails to connect to the WiFi network, ensure that the network credentials are correct and that the board is within range of the WiFi router.
- If the POST request fails, double-check the API Gateway configuration, including the endpoint URL and CORS settings.
- If the button count is not updating in DynamoDB, verify the table name and ensure that the Lambda function is correctly updating the table.

## Resources

- [Arduino Official Website](https://www.arduino.cc/)
- [AWS DynamoDB Documentation](https://aws.amazon.com/dynamodb/)
- [AWS API Gateway Documentation](https://aws.amazon.com/api-gateway/)

