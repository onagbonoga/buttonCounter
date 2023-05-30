import json
import boto3

client = boto3.client('dynamodb')
def incrementCounter(id):
    # get current value for corresponding ID
    try:
        response = client.get_item(
            TableName = 'push_count',
            Key = {
                'id':{'S':str(id)}
            }
        )
        response_item = response['Item']
        oldValue = int(response_item['countValue']['N'])

        response = client.update_item(
            TableName = 'push_count',
            Key = {
                'id':{'S':str(id)}
            },
            UpdateExpression = "SET countValue = countValue + :incrementValue",
            ExpressionAttributeValues = {':incrementValue':{'N':'1'}}
        )
    
    # if corresponding ID does not exist in table, create an entry for it
    except KeyError:
        response = client.put_item(TableName='push_count',
                            Item={
                                'id':{'S':str(id)},
                                'countValue':{'N':'1'} 
                            })
        oldValue = 0

    
    
    #print(oldValue + 1)
    return oldValue + 1
    
def lambda_handler(event, context):
    
    event_body = json.loads(event['body'])
    id = event_body['id']
    currentCount = incrementCounter(id)
    result = "*"+str(currentCount)
    return {
        'statusCode': 200,
        'body':result
    }
