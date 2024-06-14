from flask import Flask, request, jsonify

app = Flask(__name__)

@app.route('/api/sensor_data', methods=['POST'])
def receive_sensor_data():
    if request.is_json:
        data = request.get_json()
        temperature = data.get('temperature')

        if temperature is not None:
            response = {
                'status': 'success',
                'message': f'Temperature received: {temperature}'
            }
            return jsonify(response), 200
        else:
            response = {
                'status': 'fail',
                'message': 'Temperature data is missing'
            }
            return jsonify(response), 400
    else:
        response = {
            'status': 'fail',
            'message': 'Request body must be JSON'
        }
        return jsonify(response), 400

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
