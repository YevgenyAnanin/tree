


var magnifyLightboxScripts = document.getElementsByTagName('script'), magnifyLightboxDetect = navigator.userAgent.toLowerCase(), magnifyLightboxOS, magnifyLightboxBrowser, magnifyLightboxBrowserVersion;
window.domScanned = false;

if ( typeof window.magnifyLightboxes == 'undefined' ) {
	window.magnifyLightboxes = new Array();
}
if ( typeof window.magnifyPlayers == 'undefined' ) {
	window.magnifyPlayers = new Array();
}
if ( typeof window.magnifyLightboxLinks == 'undefined' ) {
	window.magnifyLightboxLinks = new Array();
}

/*-----------------------------------------------------------------------------------------------*/
var debugLog = "", debuggerLoaded = false;
logDebug = function( debugMessage ) {
	return;
	
	if ( typeof(console) !== "undefined" ) {
		console.log( debugMessage );
	}
	
	var debugArea = document.getElementById("magnifyLightboxDebugger");
	if ( debugArea ) {
		var msgs = debugArea.innerHTML;
		debugArea.innerHTML = msgs + "<br />" + debugMessage;
	} else {
		debugLog = debugLog + "<br />" + debugMessage;
	}
	
	if ( !debuggerLoaded ) {
		if ( window.addEventListener ) {
			window.addEventListener("load", function() { 
				attachDebugger();
			}, false);
		} else if ( window.attachEvent ) {
			window.attachEvent("onload", function() { 
				attachDebugger();
			});
		}
	}
}

attachDebugger = function() {
	var debugArea = document.getElementById("magnifyLightboxDebugger");
	if ( !debugArea ) {
		var debugDiv = document.createElement('div');
		debugDiv.style.overflow = "auto";
		debugDiv.style.width = "400px";
		debugDiv.style.height = "200px";
		debugDiv.id = "magnifyLightboxDebugger";
		if ( debugLog ) {
			debugDiv.innerHTML = debugLog;
		}
		document.body.appendChild(debugDiv);
		debuggerLoaded = true;
	}
}

/*-----------------------------------------------------------------------------------------------*/

/*
	Developed by Robert Nyman, http://www.robertnyman.com
	Code/licensing: http://code.google.com/p/getelementsbyclassname/
*/
var getElementsByClassName = function (className, tag, elm){
	if (document.getElementsByClassName) {
		getElementsByClassName = function (className, tag, elm) {
			elm = elm || document;
			var elements = elm.getElementsByClassName(className),
				nodeName = (tag)? new RegExp("\\b" + tag + "\\b", "i") : null,
				returnElements = [],
				current;
			for(var i=0, il=elements.length; i<il; i+=1){
				current = elements[i];
				if(!nodeName || nodeName.test(current.nodeName)) {
					returnElements.push(current);
				}
			}
			return returnElements;
		};
	}
	else if (document.evaluate) {
		getElementsByClassName = function (className, tag, elm) {
			tag = tag || "*";
			elm = elm || document;
			var classes = className.split(" "),
				classesToCheck = "",
				xhtmlNamespace = "http://www.w3.org/1999/xhtml",
				namespaceResolver = (document.documentElement.namespaceURI === xhtmlNamespace)? xhtmlNamespace : null,
				returnElements = [],
				elements,
				node;
			for(var j=0, jl=classes.length; j<jl; j+=1){
				classesToCheck += "[contains(concat(' ', @class, ' '), ' " + classes[j] + " ')]";
			}
			try	{
				elements = document.evaluate(".//" + tag + classesToCheck, elm, namespaceResolver, 0, null);
			}
			catch (e) {
				elements = document.evaluate(".//" + tag + classesToCheck, elm, null, 0, null);
			}
			while ((node = elements.iterateNext())) {
				returnElements.push(node);
			}
			return returnElements;
		};
	}
	else {
		getElementsByClassName = function (className, tag, elm) {
			tag = tag || "*";
			elm = elm || document;
			var classes = className.split(" "),
				classesToCheck = [],
				elements = (tag === "*" && elm.all)? elm.all : elm.getElementsByTagName(tag),
				current,
				returnElements = [],
				match;
			for(var k=0, kl=classes.length; k<kl; k+=1){
				classesToCheck.push(new RegExp("(^|\\s)" + classes[k] + "(\\s|$)"));
			}
			for(var l=0, ll=elements.length; l<ll; l+=1){
				current = elements[l];
				match = false;
				for(var m=0, ml=classesToCheck.length; m<ml; m+=1){
					match = classesToCheck[m].test(current.className);
					if (!match) {
						break;
					}
				}
				if (match) {
					returnElements.push(current);
				}
			}
			return returnElements;
		};
	}
	return getElementsByClassName(className, tag, elm);
};

/*-----------------------------------------------------------------------------------------------*/

getMagnifyLightboxBrowserInfo = function() {
	if ( checkMagnifyLightbox('konqueror') ) {
		magnifyLightboxBrowser = "Konqueror";
		magnifyLightboxOS = "Linux";
	}
	else if (checkMagnifyLightbox('safari')) magnifyLightboxBrowser = "Safari"
	else if (checkMagnifyLightbox('omniweb')) magnifyLightboxBrowser = "OmniWeb"
	else if (checkMagnifyLightbox('opera')) magnifyLightboxBrowser = "Opera"
	else if (checkMagnifyLightbox('webtv')) magnifyLightboxBrowser = "WebTV";
	else if (checkMagnifyLightbox('icab')) magnifyLightboxBrowser = "iCab"
	else if (checkMagnifyLightbox('msie')) magnifyLightboxBrowser = "Internet Explorer"
	else if (!checkMagnifyLightbox('compatible')) {
		magnifyLightboxBrowser = "Netscape Navigator"
		magnifyLightboxBrowserVersion = magnifyLightboxDetect.charAt(8);
	}
	else magnifyLightboxBrowser = "An unknown browser";

	if (!magnifyLightboxBrowserVersion) magnifyLightboxBrowserVersion = magnifyLightboxDetect.charAt(place + magnifyfLightboxString.length);

	if (!magnifyLightboxOS) {
		if (checkMagnifyLightbox('linux')) magnifyLightboxOS = "Linux";
		else if (checkMagnifyLightbox('x11')) magnifyLightboxOS = "Unix";
		else if (checkMagnifyLightbox('mac')) magnifyLightboxOS = "Mac"
		else if (checkMagnifyLightbox('win')) magnifyLightboxOS = "Windows"
		else magnifyLightboxOS = "an unknown operating system";
	}
	
	logDebug("got browser info, going to get magnify variables");
	getMagnifyVariables();
	
	if ( !window.domScanned ) {
		if ( window.addEventListener ) {
			window.addEventListener("load", function() { 
				scanForLinks();
			}, false);
		} else if ( window.attachEvent ) {
			window.attachEvent("onload", function() { 
				scanForLinks();
			});
		}
	}
}

scanForLinks = function() {
	if ( window.domScanned )
		return;
	
	window.magnifyLightboxLinks	= getElementsByClassName("magnify-lightbox", "a");
	window.domScanned = true;
	
	for ( var i=0; i < window.magnifyLightboxLinks.length; i++ ) {
		var el = window.magnifyLightboxLinks[i];
		createLinkPlayer( el );
	}
}

createLinkPlayer = function( el ) {
	try {
		magnifyLightboxWindow = window.top;	
		var player = new magnifyPlayer();
			
		var link = document.createElement('a');
		link.href = el.href;
		
		var attrs = new Array();
		var queryString = link.search ? link.search.slice(1, link.search.length) : "";
		var q =  queryString ? queryString.split('&') : new Array();
		var autoplaySet = false;
		for ( var i=0; i < q.length; i++ ) {
			var kv = q[i].split('=');
			if ( attrs[0] == 'init_autoplay' ) {
				autoplaySet = true;
			}
			attrs.push(kv[0], kv[1]);
		}
		if ( !autoplaySet ) {
			attrs.push('init_autoplay', '1');
		}
		var playerAttrs = attrs.join('/');
		logDebug(playerAttrs);
		
		player.hostname = link.hostname;
		player.pathname = link.pathname;
		if ( !player.pathname.match(/^\//) ) {
			player.pathname = "/" + player.pathname;
		}
		player.channel = "5TKQP712FQK6LLQ4";		
		player.url = 'http://' + player.hostname + player.pathname + "/player/svp/layout/lightbox/" + playerAttrs;
		logDebug("player url: " + player.url);
					
		player.count = magnifyPlayers.length;
		logDebug("adding player " + player.count + " " + el.href);
		
		player.url += "/player_count/" + player.count;
		
		player.info = player.createPlayer();
		magnifyPlayers.push( player );	
			
		if ( !document.getElementById('magnify_lightbox_overlay') ) {		
			var bod = magnifyLightboxWindow.document.getElementsByTagName('body')[0];
			var children = bod.childNodes;
		
			var overlay = magnifyLightboxWindow.document.createElement('div');
			overlay.id = 'magnify_lightbox_overlay';
			overlay.style.display = "none";
			overlay.style.top = "0px";
			overlay.style.left = "0px";
			overlay.style.width = "100%";
			overlay.style.height = "100%";
			overlay.style.zIndex = 5000;
			overlay.style.textAlign = "center";
			if ( magnifyLightboxBrowser == "Internet Explorer" ) {
				overlay.style.position = "absolute";
			} else {
				overlay.style.position = "fixed";
			}
			overlay.style.backgroundImage = "url('http://decor.waywire.com/decor/live/lightbox.png')";
		
			if ( children.length > 0 ) {
				bod.insertBefore(overlay, children[0]); 
			} else {
				bod.appendChild(overlay);
			}
		}
		
		
		magnifyLightboxes.push( new magnifyLightbox() );
		//el.href = "#";
		
		if ( el.addEventListener ) {
			el.addEventListener("click", function(e) { return populateMagnifyLightbox(e, player.count); }, false);
			logDebug("adding overlay event listener");
		} else if ( el.attachEvent ) {
			logDebug("attaching overlay event");
			el.attachEvent("onclick", function(e) { return populateMagnifyLightbox(e, player.count); });
		} else {
			logDebug("falling through to default overlay onclick");
			el.onclick = function(e) { return populateMagnifyLightbox(e, player.count); }
		}
			
		if (window.addEventListener) {
			window.addEventListener("message", receiveMessage, false);
		} else if (window.attachEvent) {
			window.attachEvent("onmessage", receiveMessage);
		}
	} catch(e) { logDebug("Could not create lightbox link: " + e.message); }
}

checkMagnifyLightbox = function(string) {
	place = magnifyLightboxDetect.indexOf(string) + 1;
	magnifyfLightboxString = string;
	return place;
}

getMagnifyVariables = function() {
	if ( !"" ) {
		return;
	}

	try {
		magnifyLightboxWindow = window.top;
		try {
			magnifyLightboxWindow.innerWidth;
		} catch(e) {
			magnifyLightboxWindow = window;
		}
		var myMagnifyLightboxScript = magnifyLightboxScripts[ magnifyLightboxScripts.length - 1 ];
		
		var customImg = typeof(window.lightboxCustomImage) != 'undefined' ? window.lightboxCustomImage : "";
		var imgWidth = typeof(window.lightboxCustomImageWidth) != 'undefined' ? window.lightboxCustomImageWidth : "";
		var imgHeight = typeof(window.lightboxCustomImageHeight) != 'undefined' ? window.lightboxCustomImageHeight : "";
		var showOverlay = typeof(window.showLightboxOverlay) != 'undefined' ? window.showLightboxOverlay : true;
		var showTitle = typeof(window.showLightboxTitle) != 'undefined' ? window.showLightboxTitle : true;
		var showDescription = typeof(window.showLightboxDescription) != 'undefined' ? window.showLightboxDescription : true;
		
		window.lightboxCustomImage = undefined;
		window.lightboxCustomImageWidth = undefined;
		window.lightboxCustomImageHeight = undefined;
		window.showLightboxOverlay = undefined;
		window.showLightboxTitle = undefined;
		window.showLightboxDescription = undefined;
		
		var link = document.createElement('a');
		link.href = myMagnifyLightboxScript.src;
		

		var player = new magnifyPlayer();
		player.script = myMagnifyLightboxScript;
		player.hostname = link.hostname;
		player.cid = "";
		player.title = showTitle ? '' : "";
		player.showOverlay = showOverlay;
		player.description = showDescription ? '' : "";
		player.imgWidth = imgWidth || 300;
		player.imgHeight = imgHeight || 225;
		player.img = customImg || "";
		player.channel = "5TKQP712FQK6LLQ4";
		player.url = "http://" + player.hostname + "/video//player/svp/layout/lightbox/init_autoplay/1/";

		
		player.addMagnifyLightboxMarkup();
		
		if (window.addEventListener) {
			window.addEventListener("message", receiveMessage, false);
		} else if (window.attachEvent) {
			window.attachEvent("onmessage", receiveMessage);
		}
	} catch(e) { logDebug( "problem getting magnify variables " + e.message ); }
}

mvpInjectScript = function(url, scriptId, lookFor, callback) {
	var head = document.getElementsByTagName('head')[0];
	var prevScript = document.getElementById(scriptId);
	
	if ( prevScript ) {
		head.removeChild( prevScript );
	}
	prevScript = null;
	if ( !prevScript || !(url == prevScript.src) ) {
		var oScript = document.createElement("script"); 
		oScript.setAttribute("src", url); 
		oScript.setAttribute("id", scriptId); 
		if ( callback ) {
			mvpWaitForLoad(lookFor, callback);
		}
		
		head.appendChild(oScript);
	} else {
		callback();
	}
}

mvpWaitForLoad = function(lookFor, callback) {
	var interval = setInterval(function() {
		if ( eval("typeof " + lookFor) != 'undefined') {
			clearInterval(interval);
			callback();
		}
	}, 50);					
}

function f_clientWidth() {
	return f_filterResults (
		window.innerWidth ? window.innerWidth : 0,
		document.documentElement ? document.documentElement.clientWidth : 0,
		document.body ? document.body.clientWidth : 0
	);
}

function f_clientHeight() {
	return f_filterResults (
		window.innerHeight ? window.innerHeight : 0,
		document.documentElement ? document.documentElement.clientHeight : 0,
		document.body ? document.body.clientHeight : 0
	);
}

function f_scrollLeft() {
	return f_filterResults (
		window.pageXOffset ? window.pageXOffset : 0,
		document.documentElement ? document.documentElement.scrollLeft : 0,
		document.body ? document.body.scrollLeft : 0
	);
}

function f_scrollTop() {
	return f_filterResults (
		window.pageYOffset ? window.pageYOffset : 0,
		document.documentElement ? document.documentElement.scrollTop : 0,
		document.body ? document.body.scrollTop : 0
	);
}

function f_filterResults(n_win, n_docel, n_body) {
	var n_result = n_win ? n_win : 0;
	if (n_docel && (!n_result || (n_result > n_docel)))
		n_result = n_docel;
	return n_body && (!n_result || (n_result > n_body)) ? n_body : n_result;
}

/*-----------------------------------------------------------------------------------------------*/

function magnifyLightbox() { };
magnifyLightbox.prototype = {
	yPos : 0,
	xPos : 0,
	
	// Turn everything on - mainly the IE fixes
	activate: function(){
		if (magnifyLightboxBrowser == 'Internet Explorer'){
			this.getScroll();
			this.prepareIE('100%', 'hidden');
			this.setScroll(0,0);
		}
		// this is custom opacity code
		//this.makeOpaque(true);
		this.hideSelects('hidden');
		this.displayLightbox("block");
	},
	
	// Ie requires height to 100% and overflow hidden or else you can scroll down past the lightbox
	prepareIE: function(height, overflow){
		bod = document.getElementsByTagName('body')[0];
		bod.style.height = height;
		bod.style.overflow = overflow;
  
		htm = document.getElementsByTagName('html')[0];
		htm.style.height = height;
		htm.style.overflow = overflow; 
	},
	
	// In IE, select elements hover on top of the lightbox
	hideSelects: function(visibility){
		var selects = document.getElementsByTagName('select');
		for(i = 0; i < selects.length; i++) {
			selects[i].style.visibility = visibility;
		}
		var flash = document.getElementsByTagName('object');
		for(i = 0; i < flash.length; i++) {
			flash[i].style.visibility = visibility;
		}
		var embed = document.getElementsByTagName('embed');
		for(i = 0; i < embed.length; i++) {
			embed[i].style.visibility = visibility;
		}
	},
	
	makeOpaque: function(bool) {	
		var hh = document.getElementsByTagName('head')[0];
		var removeOpacity = document.getElementById('removeOpacity');
		if ( bool ) {
			var ss = document.createElement('style');
			ss.id = "removeOpacity";
			var def = 'html, body, div, span, applet, object, iframe, ' +
			'h1, h2, h3, h4, h5, h6, p, blockquote, pre, ' +
			'a, abbr, acronym, address, big, cite, code, ' +
			'del, dfn, em, font, img, ins, kbd, q, s, samp, ' +
			'small, strike, strong, sub, sup, tt, var, ' +
			'dl, dt, dd, ol, ul, li, ' +
			'fieldset, form, label, legend, ' +
			'table, caption, tbody, tfoot, thead, tr, th, td {opacity:1.0; filter:alpha(opacity=100)}';
			ss.setAttribute("type", "text/css");
			if (ss.styleSheet) {   // IE
				ss.styleSheet.cssText = def;
			} else {                // the world
				var tt = document.createTextNode(def);
				ss.appendChild(tt);
			}
			hh.appendChild(ss);
		} else {
			if ( removeOpacity ) {
				hh.removeChild(removeOpacity);
			}
		}
	},
	
	// Taken from lightbox implementation found at http://www.huddletogether.com/projects/lightbox/
	getScroll: function(){
		if (self.pageYOffset) {
			this.yPos = self.pageYOffset;
		} else if (document.documentElement && document.documentElement.scrollTop){
			this.yPos = document.documentElement.scrollTop; 
		} else if (document.body) {
			this.yPos = document.body.scrollTop;
		}
	},
	
	setScroll: function(x, y) {
		window.scrollTo(x, y); 
	},
	
	displayLightbox: function(display){
		document.getElementById('magnify_lightbox_overlay').style.display = display;
	},
	
	// Example of creating your own functionality once lightbox is initiated
	deactivate: function() {
		var overlay = document.getElementById('magnify_lightbox_overlay');
		var old = document.getElementById('magnify_lightbox_content');
		if ( old )
			overlay.removeChild(old);
		overlay.innerHTML = "";
		
		if ( magnifyLightboxBrowser == "Internet Explorer" ) {
			this.prepareIE("auto", "auto");
			this.setScroll(0,this.yPos);
		}
		
		this.hideSelects("visible");
		this.makeOpaque(false);
		this.displayLightbox("none");
		return false;
	}
}

/*-----------------------------------------------------------------------------------------------*/

populateMagnifyLightbox = function(e, count) {
	try {
		var player = magnifyPlayers[count];
		var lightbox = magnifyLightboxes[count];
	
		var info = player.info;
		
		var overlay = document.getElementById('magnify_lightbox_overlay');		
		overlay.appendChild(info);				
		overlay.onclick = function() { lightbox.deactivate(); }
		
		var closeButton = document.getElementById('magnify_lightbox_close_' + count);
		closeButton.onclick = function() { return lightbox.deactivate(); }
		
		lightbox.activate();
		stopEvent(e);
		//return false;
	} catch(e) { logDebug( "can't spawn lightbox: " + e.message ); }
}

stopEvent  = function(e) {
	if(!e) var e = window.event;
	e.cancelBubble = true;
	e.returnValue = false;
	if (e.stopPropagation) {
		e.stopPropagation();
		e.preventDefault();
	}
	return false;
}

receiveMessage = function(event) {
	var eventArray = event.data.split(':');
	var count = eventArray[0];
	var data = eventArray[1];
	var player = magnifyPlayers[count];	
	
	if ( ! (count && data && player ) ) {
		return;
	}
	
	logDebug("Received message: count: " + count + " data: " + data + " player: " + player);	
	
	var myMagnifyLightboxScript = magnifyLightboxScripts[ magnifyLightboxScripts.length - 1 ];
	var link = document.createElement('a');
	link.href = myMagnifyLightboxScript.src;
	var hostname = player.hostname || link.hostname;
	
	if ( event.origin !== "http://" + hostname ) {
		return;
	}
	if ( data.match("resize") ) {
		logDebug("resizing player");
		resizeOverlay(data.split(" ")[1]);
	}
}

resizeOverlay = function(szString) {
	try {
		var measurements = szString.split('x');
		var frameContainer = document.getElementById('magnify_lightbox_content');
		var currentDimensions = [ parseInt( frameContainer.style.width ), parseInt( frameContainer.style.height ) ];
		
		logDebug("current player height: " + currentDimensions[0] + " current player width: " + currentDimensions[1]);
		
		frameContainer.style.width = measurements[0] + "px";
		frameContainer.style.height = measurements[1] + "px";
		
		frameContainer = document.getElementById('magnify_player_iframe_container');
		frameContainer.style.width = measurements[0] + "px";
		frameContainer.style.height = measurements[1] + "px";
		
		
		logDebug("new player height: " + frameContainer.style.height + " new player width: " + frameContainer.style.width);
	} catch(e) { logDebug( "Error in resize overlay: " +  e.message ) }
}

function magnifyPlayer() { };
magnifyPlayer.prototype = {
	addMagnifyLightboxMarkup: function() {
		
		if ( !this.cid || !this.img || !this.channel ) {
			logDebug("cid: " + cid + " img: " + img + " channel: " + channel);
			return;
		}
	
		logDebug("adding markup");
		try {
			var bod = magnifyLightboxWindow.document.getElementsByTagName('body')[0];
			var children = bod.childNodes;
		
			if ( !document.getElementById('magnify_lightbox_overlay') ) {	
				var overlay = magnifyLightboxWindow.document.createElement('div');
				overlay.id = 'magnify_lightbox_overlay';
				overlay.style.display = "none";
				overlay.style.top = "0px";
				overlay.style.left = "0px";
				overlay.style.width = "100%";
				overlay.style.height = "100%";
				overlay.style.zIndex = 5000;
				overlay.style.textAlign = "center";
				if ( magnifyLightboxBrowser == "Internet Explorer" ) {
					overlay.style.position = "absolute";
				} else {
					overlay.style.position = "fixed";
				}
				overlay.style.backgroundImage = "url('http://decor.waywire.com/decor/live/lightbox.png')";
			
				if ( children.length > 0 ) {
					bod.insertBefore(overlay, children[0]); 
				} else {
					bod.appendChild(overlay);
				}
			}
			
			this.count = magnifyPlayers.length;
			this.url += "/player_count/" + this.count;
			this.info = this.createPlayer();
			magnifyPlayers.push( this );
			
			magnifyLightboxes.push( new magnifyLightbox() );
			
			logDebug("about to build lightbox");
			
			this.buildMagnifyLightbox();
		} catch(e) { logDebug( e.message ); }
	},
	
	createPlayer: function() {
		var windowWidth = parseInt(f_clientWidth());
		var windowHeight = parseInt(f_clientHeight());
		
		var playerWidth = this.getPlayerWidth( windowWidth );
		var playerHeight = Math.floor( playerWidth * .5625 ) + 200;
	
		var topMargin = Math.floor((windowHeight - playerHeight) / 2) > 0 ? Math.floor((windowHeight - playerHeight) / 2) : 10;
		
		var ss = document.createElement('style');
		var def = '';
		ss.setAttribute("type", "text/css");
		if (ss.styleSheet) {   // IE
			ss.styleSheet.cssText = def;
		} else {                // the world
			var tn = document.createTextNode(def);
			ss.appendChild(tn);
		}	
	
		var div = document.createElement('div');
		div.className = 'magnify-player-lightbox-content';
		div.id = 'magnify_lightbox_content';
		div.style.position = 'relative';
		div.style.width = playerWidth + 'px';
		div.style.height = playerHeight + 'px';
		div.style.marginTop = topMargin + 'px';
		div.style.marginLeft = 'auto';
		div.style.marginRight = 'auto';
		div.style.textAlign = 'left';
		
		var closeDiv = document.createElement('div');
		closeDiv.className = 'magnify-player-close clearfix';
		closeDiv.style.textAlign = 'right';
		closeDiv.style.position = 'absolute';
		closeDiv.style.width = "100%";
		closeDiv.style.zIndex = 2;
		
		var closeButton = document.createElement('a');
		closeButton.href = "#";
		closeButton.className = 'magnify-player-close-link';
		closeButton.id = 'magnify_lightbox_close_' + this.count;
		closeButton.style.position = 'relative';
		
		var closeImg = document.createElement('img'); 
		closeImg.src = 'http://www.waywire.com/decor/live/lightbox_close.png';
		closeImg.border = '0';
		closeImg.style.position = 'relative';
		closeImg.style.top = '-15px';
		closeImg.style.right = '-15px';
		
		var iframeDiv = document.createElement('div');
		iframeDiv.style.position = 'relative';
		iframeDiv.style.height = playerHeight + 'px';
		iframeDiv.style.width = playerWidth + 'px';
		iframeDiv.id = "magnify_player_iframe_container";
		
		var iframe = document.createElement('iframe');
		iframe.id = 'magnify_lightbox_frame_' + this.count;
		iframe.src = this.url;
		iframe.style.width = "100%";
		iframe.style.height = "100%";
		iframe.frameBorder = "0";
		iframe.marginHeight = "0";
		iframe.marginWidth = "0";
		iframe.scrolling = "no";
		iframe.setAttribute('allowTransparency', true);
		
		closeButton.appendChild(closeImg);
		closeDiv.appendChild(closeButton);
		iframeDiv.appendChild(iframe);
		div.appendChild(ss);
		div.appendChild(closeDiv);
		div.appendChild(iframeDiv);
		
		return div;
	},

	insertMagnifyLightboxCSS: function( cssString ) {
		var ss = document.createElement('style');
		var def = cssString;
		ss.setAttribute("type", "text/css");
		if (ss.styleSheet) {   // IE
			ss.styleSheet.cssText = def;
		} else {                // the world
			var tn = document.createTextNode(def);
			ss.appendChild(tn);
		}
		var head = document.getElementsByTagName('head')[0];
		head.appendChild(ss);	
	},
	
	getPlayerWidth: function(windowWidth) {
		if ( windowWidth < 750 ) {
			return 600;
		} else {
			// this produces a player width of 720
			return 736;
		}
	},
	
	buildMagnifyLightbox: function() {	
		logDebug("starting to build lightbox");
		if ( !this.cid || !this.img || !this.channel ) {
			logDebug("missing something, aborting");
			logDebug("cid: " + cid + " img: " + img + " channel: " + channel);
			return;
		}
	
		this.buildMagnifyBug();
	},
	
	buildMagnifyBug: function() {
		logDebug("building magnify bug");
		try {	
			var player = this;
			var container = document.createElement('div');
			container.id = "magnify_lightbox_player_overlay_container_" + this.count;
			container.className = "magnify-lightbox-overlay-container";		
			
			if ( this.showOverlay ) {		
				var overlay = document.createElement('div');
				overlay.id = "magnify_lightbox_player_overlay_" + this.count;
				overlay.className = "magnify-lightbox-overlay";
				overlay.style.backgroundImage = "url(http://static.media.magnify.net.s3.amazonaws.com/img/player_overlay.png)";
				overlay.style.backgroundPosition = "center center";
				overlay.style.backgroundRepeat = "no-repeat";
				overlay.style.cursor = "pointer";
				overlay.style.cursor = "hand";
				overlay.style.position = "absolute";
				overlay.style.left = "0px";
				overlay.style.width = this.imgWidth + "px";
				overlay.style.height = this.imgHeight + "px";
				
				var overlayImg = document.createElement('img'); 
				overlayImg.src = "http://decor.waywire.com/decor/live/transparent.gif";	
				overlayImg.width = this.imgWidth;
				overlayImg.height = this.imgHeight;
				overlay.appendChild(overlayImg);
				
				container.appendChild(overlay);
				
				if ( overlay.addEventListener ) {
					overlay.addEventListener("click", function(e) { return populateMagnifyLightbox(e, player.count); }, false);
					logDebug("adding overlay event listener");
				} else if ( overlay.attachEvent ) {
					logDebug("attaching overlay event");
					overlay.attachEvent("onclick", function(e) { return populateMagnifyLightbox(e, player.count); });
				} else {
					logDebug("falling through to default overlay onclick");
					overlay.onclick = function(e) { return populateMagnifyLightbox(e, player.count); }
				}
			}
			
			var thumbnailContainer = document.createElement('div');
			thumbnailContainer.id = "magnify_lightbox_player_thumbnail_container_" + this.count;
			thumbnailContainer.className = "magnify-lightbox-thumbnail-container";
			thumbnailContainer.style.cursor = "pointer";
			thumbnailContainer.style.cursor = "hand";
			
			var thumbnail = document.createElement('img');
			thumbnail.className = "magnify-lightbox-thumbnail";
			thumbnail.src = this.img;
			thumbnail.width = this.imgWidth;
			thumbnail.height = this.imgHeight;
			
			thumbnailContainer.appendChild(thumbnail);
			container.appendChild(thumbnailContainer);
			
			if ( !this.showOverlay ) {			
				if ( thumbnailContainer.addEventListener ) {
					logDebug("adding thumbnail event listener");
					thumbnailContainer.addEventListener("click", function(e) { return populateMagnifyLightbox(e, player.count); }, false);
				} else if ( thumbnailContainer.attachEvent ) {
					logDebug("attaching thumbnail event");
					thumbnailContainer.attachEvent("onclick", function(e) { return populateMagnifyLightbox(e, player.count); });
				} else {
					logDebug("falling through to default onclick");
					thumbnailContainer.onclick = function(e) { return populateMagnifyLightbox(e, player.count); }
				}
			}
			
			logDebug("events come after DOM attachment");
			
			if ( this.title ) {
				var titleDiv = document.createElement('div');
				titleDiv.className = "magnify-lightbox-player-title";
				titleDiv.innerHTML = this.title;
				container.appendChild(titleDiv);
			}
			
			if ( this.description ) {
				var descDiv = document.createElement('div');
				descDiv.className = "magnify-lightbox-player-description";
				descDiv.innerHTML = this.description;
				container.appendChild(descDiv);		
			}
						
			var div = document.createElement("div");
			div.id = "magnify_lightbox_player_" + this.count;
			div.className = "magnify-lightbox-player";
			div.style.position = "relative";
			div.appendChild(container);
			this.bug = div;
			this.insertMagnifyLightbox();
		} catch(e) { logDebug( e.message ); }
	},
	
	insertMagnifyLightbox: function() {
		logDebug("inserting bug before " + this.script.src);
		this.script.parentNode.insertBefore(this.bug, this.script);
	}
}

getMagnifyLightboxBrowserInfo();
