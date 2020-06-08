         // Called after form input is processed
         function startConnect() 
         {
            // Generate a random client ID
            clientID = "clientID-" + parseInt(Math.random() * 100);
            host = "test.mosquitto.org";
            port = "8080";
            // Initialize new Paho client connection
            client = new Paho.MQTT.Client(host, Number(port), clientID);
            // Set callback handlers
            client.onConnectionLost = onConnectionLost;
            client.onMessageArrived = onMessageArrived;
            // Connect the client, if successful, call onConnect function
            client.connect({ 
                onSuccess: onConnect,
            });
        }
        // Called when the client connects
        function onConnect() 
        {

        }
        // Called when the client loses its connection
        function onConnectionLost(responseObject) 
        {
            document.getElementById("messages").innerHTML += '<span>ERROR: Connection lost</span><br/>';
            if (responseObject.errorCode !== 0) 
            {
                document.getElementById("messages").innerHTML += '<span>ERROR: ' + + responseObject.errorMessage + '</span><br/>';
            }
        }
        // Called when a message arrives
        function onMessageArrived(message) 
        {
            console.log("onMessageArrived: " + message.payloadString);
            document.getElementById("messages").innerHTML += '<span>Topic: ' + message.destinationName + '  | ' + message.payloadString + '</span><br/>';
        }

        // Called when the disconnection button is pressed
        function startDisconnect() 
        {
            client.disconnect();
            document.getElementById("messages").innerHTML += '<span>Disconnected</span><br/>';
        }
        function TurnSafetyOn()
        {
            var text = document.getElementById("safety").textContent;
            if (text == "Turn Safety On")
            {
                document.getElementById("safety").textContent = "Turn Safety Off" 
                message = new Paho.MQTT.Message("TurnSystemON")
                message.destinationName = "TurnSystemfortheProject123"
                client.send(message)
            }
            if (text == "Turn Safety Off")
            {
                document.getElementById("safety").textContent = "Turn Safety On" 
                message = new Paho.MQTT.Message("TurnSystemOFF")
                message.destinationName = "TurnSystemfortheProject123"
                client.send(message)
            }
        }
        function TurnSecurityOn()
        {
            var text = document.getElementById("security").textContent;
            if (text == "Turn Security On")
            {
                document.getElementById("security").textContent = "Turn Security Off" 
                message = new Paho.MQTT.Message("TurnSecurityON")
                message.destinationName = "TurnSystemfortheProject123"
                client.send(message)
            }
            if (text == "Turn Security Off")
            {
                document.getElementById("security").textContent = "Turn Security On" 
                message = new Paho.MQTT.Message("TurnSecurityOFF")
                message.destinationName = "TurnSystemfortheProject123"
                client.send(message)
            }
        }
        function Exhaust()
        {
            var text = document.getElementById("exhaust").textContent;
            if (text == "Turn Exhaust On")
            {
                document.getElementById("exhaust").textContent = "Turn Exhaust Off" 
                message = new Paho.MQTT.Message("TurnExhaustON")
                message.destinationName = "ExhaustCode14512"
                client.send(message)
            }
            else
            {
                document.getElementById("exhaust").textContent = "Turn Exhaust On" 
                message = new Paho.MQTT.Message("TurnExhaustOFF")
                message.destinationName = "ExhaustCode14512"
                client.send(message)
            }
        }
