const lista = document.getElementById("lista");
const light_theme = ["black", "rgb(228, 255, 194)"];
const dark_theme = ["white", "rgb(31, 36, 25)"];
const storage_key = "localtheme"; 
let current = 0;
async function getFile() {
    const response = await fetch("./list.json");
    if (!response.ok) {
        throw new Error("File not found");
    }
    const data = await response.json();
    return data;
}

async function updateList() {
    setTheme(getCurrentTheme());
    file = await getFile();
    if (lista != null && file != null) {
        file.forEach(item => {
            var name = item.name;
            var state = item.state
            var element = document.createElement("li");
            element.innerHTML = `<strong>${name}</strong>: 
            ${state ? "<span style=\"color:green;\">Complete</span>" : "<span style=\"color:magenta;\">Pending</span>"}`;
            lista.appendChild(element);
        });
    }
}

function getCurrentTheme() {
    if (localStorage.getItem(storage_key) == null) {
        localStorage.setItem(storage_key,0);
        return localStorage.getItem(storage_key);
    } 
    else {
        return localStorage.getItem(storage_key);
    }
}

function setCurrentTheme(val) {
    localStorage.setItem(storage_key,val);
}

function setTheme(val) {
    if (val == 0) {
        document.body.style.backgroundColor = light_theme[1];
        document.body.style.color = light_theme[0];
    } else {
        document.body.style.backgroundColor = dark_theme[1];
        document.body.style.color = dark_theme[0];
    }
}

function changeTheme() {
    if (getCurrentTheme() == 1) {
        setTheme(0);
        setCurrentTheme(0);
    } else {
        setTheme(1);
        setCurrentTheme(1);
    }
}
updateList();