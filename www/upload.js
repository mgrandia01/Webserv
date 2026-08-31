const input = document.getElementById("photo-upload");
const button = document.getElementById("upload-button");
const deleteButtons = document.querySelectorAll(".delete-photo");

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

deleteButtons.forEach(button => {

	button.addEventListener("click", async () => {

	const imageName = button.dataset.image;

	const response = await fetch(`/uploads/${imageName}`, {
		method: "DELETE"
	});

	if (response.ok) {
		button.parentElement.remove();
	} else {
		console.log("Error deleting image");
	}
	});

});
