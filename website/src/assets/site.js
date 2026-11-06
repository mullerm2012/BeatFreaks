// Turns store buttons on or off from site-config.js, and draws the
// decorative waveform on the home page.
(function () {
  var cfg = window.BEATFREAKS_STORE || {};
  var open = typeof cfg.checkoutUrl === "string" && cfg.checkoutUrl.indexOf("https://") === 0;

  document.querySelectorAll("[data-buy]").forEach(function (a) {
    if (open) {
      a.href = cfg.checkoutUrl + (cfg.checkoutUrl.indexOf("?") < 0 ? "?embed=1" : "&embed=1");
      a.classList.add("lemonsqueezy-button");
      a.removeAttribute("aria-disabled");
      a.textContent = a.getAttribute("data-buy") || "Buy BeatFreaks";
    } else {
      a.removeAttribute("href");
      a.setAttribute("aria-disabled", "true");
      a.textContent = "Store opens at launch";
    }
  });
  document.querySelectorAll("[data-orders]").forEach(function (a) {
    a.href = cfg.ordersUrl || "https://app.lemonsqueezy.com/my-orders";
    if (!open) {
      a.setAttribute("aria-disabled", "true");
      a.title = "Available after the store opens";
    }
  });
  document.querySelectorAll("[data-beta]").forEach(function (a) {
    if (cfg.betaReady) {
      a.href = cfg.betaUrl;
    } else {
      a.removeAttribute("href");
      a.setAttribute("aria-disabled", "true");
      a.textContent = "Beta download coming soon";
    }
  });
  document.querySelectorAll("[data-price]").forEach(function (el) {
    el.textContent = cfg[el.getAttribute("data-price")] || el.textContent;
  });
  if (open) {
    var s = document.createElement("script");
    s.src = "https://app.lemonsqueezy.com/js/lemon.js";
    s.defer = true;
    document.body.appendChild(s);
  }

  var w = document.getElementById("wave");
  if (w) {
    for (var i = 0; i < 64; i++) {
      var b = document.createElement("i");
      b.style.height = Math.min(100, 20 + Math.abs(Math.sin(i * 0.55) * 60) + (i % 4 === 0 ? 18 : 0)) + "%";
      if (i < 26) b.style.background = "#C6FF00";
      w.appendChild(b);
    }
  }
})();
