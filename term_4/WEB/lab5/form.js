document.addEventListener("DOMContentLoaded", function() {
  const form = document.getElementById('myForm');
  const dataArray = [];

  // Initialize field validation on input
  document.getElementById('sender').addEventListener('input', validateSender);
  document.getElementById('startTime').addEventListener('input', validateStartTime);
  document.getElementById('endTime').addEventListener('input', validateEndTime);
  document.getElementById('weight').addEventListener('input', validateWeight);
  document.getElementById('receiver').addEventListener('input', validateReceiver);

  form.addEventListener('submit', function(event) {
    event.preventDefault();
    if (validateAllFields()) {
      const senderField = document.getElementById('sender');
      const startTimeField = document.getElementById('startTime');
      const endTimeField = document.getElementById('endTime');
      const weightField = document.getElementById('weight');
      const receiverField = document.getElementById('receiver');
      
      addToDataArray(
        senderField.value, 
        startTimeField.value, 
        endTimeField.value, 
        weightField.value, 
        receiverField.value
      );
      clearForm();
    }
  });

  function validateAllFields() {
    const isSenderValid = validateSender();
    const isStartTimeValid = validateStartTime();
    const isEndTimeValid = validateEndTime();
    const isWeightValid = validateWeight();
    const isReceiverValid = validateReceiver();

    return isSenderValid && isStartTimeValid && isEndTimeValid && 
           isWeightValid && isReceiverValid;
  }

  function validateSender() {
    const senderField = document.getElementById('sender');
    const senderValue = senderField.value.trim();
    const errorElement = document.getElementById('senderError');
    
    if (senderValue.length >= 20 || !/\d$/.test(senderValue)) {
      errorElement.textContent = 'The "Sender\'s address" field must be text less than 20 characters long and end with a number';
      return false;
    }
    
    errorElement.textContent = '';
    return true;
  }

  function validateStartTime() {
    const startTimeField = document.getElementById('startTime');
    const timePattern = /^(0?[0-9]|1[0-9]|2[0-3]):[0-5][0-9]$/;
    const errorElement = document.getElementById('startTimeError');
    
    if (!timePattern.test(startTimeField.value)) {
      errorElement.textContent = '"Pick-up Time" field must be in hh:mm format (e.g. 10:30)';
      return false;
    }
    
    errorElement.textContent = '';
    return true;
  }

  function validateEndTime() {
    const endTimeField = document.getElementById('endTime');
    const timePattern = /^(0?[0-9]|1[0-9]|2[0-3]):[0-5][0-9]$/;
    const errorElement = document.getElementById('endTimeError');
    
    if (!timePattern.test(endTimeField.value)) {
      errorElement.textContent = '"Drop-off Time" field must be in hh:mm format (e.g. 10:30)';
      return false;
    }
    
    errorElement.textContent = '';
    return true;
  }

  function validateWeight() {
    const weightField = document.getElementById('weight');
    const weightValue = weightField.value.trim();
    const errorElement = document.getElementById('weightError');
    
    if (weightValue === '' || weightValue < 1 || weightValue > 100) {
      errorElement.textContent = '"Weight of package" field must be a number from 1 to 100';
      return false;
    }
    
    errorElement.textContent = '';
    return true;
  }

  function validateReceiver() {
    const receiverField = document.getElementById('receiver');
    const receiverValue = receiverField.value.trim();
    const errorElement = document.getElementById('receiverError');
    
    if (receiverValue.length >= 20 || !/\d$/.test(receiverValue)) {
      errorElement.textContent = 'The "Receiver\'s address" field must be text less than 20 characters long and end with a number';
      return false;
    }
    
    errorElement.textContent = '';
    return true;
  }

  function addToDataArray(sender, startTime, endTime, weight, receiver) {
    const data = {
      sender: sender,
      startTime: startTime,
      endTime: endTime,
      weight: weight,
      receiver: receiver
    };
    dataArray.push(data);
    console.log("Data added to array:", data);
  }

  function clearForm() {
    const inputs = document.querySelectorAll('input[type="text"], input[type="number"]');
    inputs.forEach(input => input.value = '');
    
    // Clear all error messages
    document.querySelectorAll('.error').forEach(errorElement => {
      errorElement.textContent = '';
    });
  }
});