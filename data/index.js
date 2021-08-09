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