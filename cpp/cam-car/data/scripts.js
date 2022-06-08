
function sendEvent(motor, direction, speed) {

    var uri;
    var data;

    if (motor == "dc") {
        url = "api/dc-motor";
        data = {
            "direction": direction,
            "speed": speed
        };
    }
    else if (motor == "servo") {
        url = "api/servo-motor";
        data = {
            "direction": direction,
            "speed": speed
        };
    }
    
    var req = new XMLHttpRequest();

    req.open("POST", url, true);

    /* Set headers */
    req.setRequestHeader(
        "Content-type", "application/json; charset=UTF-8"
    );

    req.onreadystatechange = () => {
        /* Check for errors on return */
        if (req.readyState == 4 && req.status != 200) {
            console.log("POST request failed")
        }
    }

    console.log(data);

    req.send(JSON.stringify(data));
}