class TLocalStorage {
  constructor(storageKey = 'transportReviews') {
      this.storageKey = storageKey;
  }

  addValue(key, value) {
      const data = this.getAll();
      data[key] = value;
      localStorage.setItem(this.storageKey, JSON.stringify(data));
  }

  getValue(key) {
      const data = this.getAll();
      return data[key];
  }

  deleteValue(key) {
      const data = this.getAll();
      if (data.hasOwnProperty(key)) {
          delete data[key];
          localStorage.setItem(this.storageKey, JSON.stringify(data));
          return true;
      }
      return false;
  }

  getKeys() {
      return Object.keys(this.getAll());
  }

  getAll() {
      const data = localStorage.getItem(this.storageKey);
      return data ? JSON.parse(data) : {};
      // "{'1' : 'хорошо'}" 
      // {'1' : 'хорошо'}  
  }

  clear() {
      localStorage.removeItem(this.storageKey);
  }
}

const reviewStorage = new TLocalStorage();

function showMessage(message) {
  document.getElementById('output').innerHTML = message;
}

function addReview() {
  const company = prompt("Enter transport company name:");
  if (!company) return;
  
  const review = prompt("Enter your review:");
  if (!review) return;
  
  reviewStorage.addValue(company, review);
  showMessage(`Review for <b>${company}</b> added successfully!`);
}

function deleteReview() {
  const company = prompt("Enter company name to delete review:");
  if (!company) return;
  
  if (reviewStorage.deleteValue(company)) {
      showMessage(`Review for <b>${company}</b> deleted successfully!`);
  } else {
      showMessage(`Review for <b>${company}</b> not found!`);
  }
}

function getReview() {
  const company = prompt("Enter company name to get review:");
  if (!company) return;
  
  const review = reviewStorage.getValue(company);
  if (review) {
      showMessage(`<b>${company}</b>: ${review}`);
  } else {
      showMessage(`No review found for <b>${company}</b>`);
  }
}

function listAllReviews() {
  const allReviews = reviewStorage.getAll();
  const companies = Object.keys(allReviews);
  
  if (companies.length === 0) {
      showMessage("No reviews available yet.");
      return;
  }
  
  let output = "<h3>All Reviews:</h3><ul>";
  companies.forEach(company => {
      output += `<li><b>${company}</b>: ${allReviews[company]}</li>`;
  });
  output += "</ul>";
  
  showMessage(output);
}

function clearStorage() {
  if (confirm("Are you sure you want to delete ALL reviews?")) {
      reviewStorage.clear();
      showMessage("All reviews have been deleted.");
  }
}