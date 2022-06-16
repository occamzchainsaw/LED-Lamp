function updateBrightness() {
    var label = document.getElementById("bLabel");
    var val = document.getElementById("bSlider").value;
    label.innerHTML = 'Brightness: ' + val + "%";
}

async function selectSolidColor(elmnt) {
    var buttons = document.getElementsByClassName("colourButton");
    for (i = 0; i < buttons.length; i++) {
        buttons[i].classList.remove("selected");
    }
    elmnt.classList.add("selected");
    
    try {
        const response = await fetch("http://rgb-controller.local/led", {
            method: "PUT",
            body: JSON.stringify({ effect: 'solid', option1: 'ff5e00' }),
            header: { "Content-Type": "application/json", },
        });
    } catch (error) {
        alert("Request failed - see console.");
        console.error(error);
    }
}

function openPage(pageName, elmnt) {
    var i, tabcontent;
    //  hide all divs by default
    tabcontent = document.getElementsByClassName("tabcontent");
    for (i = 0; i < tabcontent.length; i++) {
        tabcontent[i].style.display = "none";
    }

    //  show the selected div
    document.getElementById(pageName).style.display = "block";
}