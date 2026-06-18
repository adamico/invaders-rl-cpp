// Reusable quiz widget. Markup:
// <div class="quiz" data-answer="1">
//   <div class="q">Question?</div>
//   <button>option a</button><button>option b</button>...
//   <div class="fb"></div>
// </div>
// data-answer is the 0-based index of the correct button.
// Optional per-button feedback via data-fb on each button.
document.querySelectorAll('.quiz').forEach(function (quiz) {
  var answer = parseInt(quiz.dataset.answer, 10);
  var fb = quiz.querySelector('.fb');
  var buttons = Array.prototype.slice.call(quiz.querySelectorAll('button'));
  buttons.forEach(function (btn, i) {
    btn.addEventListener('click', function () {
      buttons.forEach(function (b) { b.disabled = false; b.classList.remove('correct', 'wrong'); });
      if (i === answer) {
        btn.classList.add('correct');
        fb.textContent = btn.dataset.fb || 'Correct.';
        fb.style.color = 'var(--good)';
      } else {
        btn.classList.add('wrong');
        buttons[answer].classList.add('correct');
        fb.textContent = btn.dataset.fb || 'Not quite — the highlighted answer is right.';
        fb.style.color = 'var(--bad)';
      }
    });
  });
});
