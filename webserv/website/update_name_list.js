const nameList = document.getElementById('name-list');

// Load existing names from server
fetch('/name_list.txt')
  .then(response => response.text())
  .then(text => {
    const names = text.trim().split('\n');
    for (const name of names) {
      addNameToList(name);
    }
  })
  .catch(error => {
    console.error(`Error loading name list: ${error}`);
  });

// Add new name to list when form is submitted
const addNameForm = document.getElementById('add-name-form');
addNameForm.addEventListener('submit', event => {
  event.preventDefault();
  const nameInput = document.getElementById('name-input');
  const name = nameInput.value.trim();
  if (name) {
    addNameToList(name);
    fetch('/add_name.py', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/x-www-form-urlencoded',
      },
      body: `name=${encodeURIComponent(name)}`,
    })
    .then(response => {
      if (!response.ok) {
        throw new Error(`Failed to add name: ${response.statusText}`);
      }
      nameInput.value = '';
      nameInput.focus();
    })
    .catch(error => {
      console.error(`Error adding name: ${error}`);
      alert(`Failed to add name: ${error}`);
    });
  }
});

function addNameToList(name) {
  const li = document.createElement('li');
  li.textContent = name;
  nameList.appendChild(li);
}
