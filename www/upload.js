const input = document.getElementById("photo-upload");
const button = document.getElementById("upload-button");

button.addEventListener("click", function()
{
    const file = input.files[0];

    if (!file)
        return;

    console.log("Archivo:", file.name);
    console.log("Tamaño:", file.size);
    console.log("Tipo:", file.type);

    fetch("/" + file.name, {
        method: "POST",
        body: file
    })
    .then(function(response)
    {
        console.log("Respuesta:", response.status);
    })
    .catch(function(error)
    {
        console.log("ERROR:", error);
    });
});
