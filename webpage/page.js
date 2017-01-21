(function(window){
    var document = window.document;
    
    // ==================================================
    //
    //    CARD
    //
    // ==================================================

    function Card(cardData){
        this._node = null;
        this._valueNode = null;
        this._cardData = cardData;

        this._createUI();
    }

    Card.prototype._createUI = function(){
        var titleNode;
        
        this._node = document.createElement('div');
        this._node.classList.add('card');

        titleNode = document.createElement('h2');
        titleNode.innerHTML = this._cardData.title;
        this._node.appendChild(titleNode);
            
        this._valueNode = document.createElement('p');
        this._valueNode.innerHTML = '-';
        this._node.appendChild(this._valueNode)
    }

    Card.prototype.appendTo = function(element){
        element.appendChild(this._node);
    }

    Card.prototype.update = function(data){
        this._valueNode.innerHTML = this._cardData.format(data);
    }


    // ==================================================
    //
    //    VIEW
    //
    // ==================================================
   
    function View(){
        this._cards = []; 
        this._progressValue = null;
    }

    View.prototype.init = function(cardData){
        var 
            me = this,
            body = document.body,
            header = document.createElement('h1'),
            progress = document.createElement('div'),
            cardContainer = document.createElement('div');

        header.innerHTML = document.title;
        body.appendChild(header);

        progress.classList.add('progress');
        this._progressValue = document.createElement('div');
        this._progressValue.classList.add('progress-value');
        progress.appendChild(this._progressValue);
        body.appendChild(progress);

        cardData.forEach(function(item){
            var card = new Card(item);
            card.appendTo(cardContainer);
            me._cards.push(card);
        });


        body.appendChild(cardContainer);

    }

    View.prototype.setProgress = function(value){
        this._progressValue.style.width = value + '%';
    }

    View.prototype.update = function(data){
        this._cards.forEach(function(card){
            card.update(data);
        });
    }

    // ==================================================
    //
    //    APP
    //
    // ==================================================
   
    function App () {
        this._timeout = 1;
        this._interval = 5;
        this._cardData = [
            {
                title: 'Lauke',
                format: function(data){
                    if (data && data.t1){
                        return (+data.t1).toFixed(1) + '&deg;';
                    }
                    return '-';
                }
            },
            {
                title: 'Vanduo',
                format: function(data){
                    if (data && data.t2 && data.t3){
                        return (+data.t2).toFixed(1) + '&deg; ~ ' + (+data.t3).toFixed(1) + '&deg;';
                    }
                    return '- ~ -';
                }
            },
            {
                title: 'Data',
                format: function(data){
                    if (data && data.date){
                        var parts = data.date.split(/\D/);
                        return parts[0] + '-' + parts[1] + '-' + parts[2];
                    }
                    return '-';
                }
            },
            {
                title: 'Laikas',
                format: function(data){
                    if (data && data.date){
                        var parts = data.date.split(/\D/);
                        return parts[3] + ':' + parts[4];
                    }
                    return '-';
                }
            },
            {
                title: 'Šildymas',
                format: function(data){
                    if (data && data.t4 && data.s11){
                        return (+data.t4).toFixed(1) + '&deg; / ' + (+data.s11).toFixed(1) + '&deg;';
                    }
                    return '-';
                }
            }
        ];
        this._view = new View();
        
    }

    App.prototype.start = function() {
        var me = this;

        this._view.init(this._cardData);

        window.setInterval(me._onTick.bind(me), 1000);
    }

    App.prototype._onTick = function(){
        this._timeout--;
        this._updateProgress();

        if (this._timeout > 0){
            return;
        }

        this._timeout = this._interval;
        this._getData();
    }

    App.prototype._updateProgress = function(){
        this._view.setProgress(this._timeout * 100 / this._interval);
    }

    App.prototype._getData = function(){
        var me = this;
        var request = new window.XMLHttpRequest();

        request.onreadystatechange = function() {
            if (request.readyState === 4) {
                if (request.status === 200) { 
                    //if (callback){
                        me._onDataReveived.call(me, JSON.parse(request.responseText));
                    //}
                } else {
                    console.log('An error occurred during your request: ',  request.status, request.statusText);
                } 
            }
        }

        request.open('GET', '/data.json');
        request.send();
    }

    App.prototype._onDataReveived = function(data){
        this._view.update(data);
    }
    
    var app = new App();
    app.start();
    
})(this)
