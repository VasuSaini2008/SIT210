let lightStates = {
    "living room": false,
    "bathroom": false,
    "closet": false
};


function toggleLight(room) {

    // Change the local state
    lightStates[room] = !lightStates[room];

    let state = lightStates[room];

    // Update webpage
    updateWebPage(room, state);

    // Display message
    document.getElementById("message").innerText =
        room + " light is now " + (state ? "ON" : "OFF");

    /*
       Arduino Cloud/API call goes here.

       Example:

       sendToArduinoCloud(room, state);
    */

    console.log("Room:", room);
    console.log("State:", state);
}


function updateWebPage(room, state) {

    let element;

    if (room === "living room") {
        element = document.getElementById("livingLight");
    }

    else if (room === "bathroom") {
        element = document.getElementById("bathroomLight");
    }

    else if (room === "closet") {
        element = document.getElementById("closetLight");
    }


    if (state) {

        element.classList.remove("off");
        element.classList.add("on");

        element.innerText = "ON";

    } else {

        element.classList.remove("on");
        element.classList.add("off");

        element.innerText = "OFF";
    }
}