(function(window){
    var 
        document = window.document,
        createElement = document.createElement.bind(document),
        deg = '&deg;';

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

        this._node = createElement('div');
        this._node.classList.add('card');

        titleNode = createElement('h2');
        titleNode.innerHTML = this._cardData.title;
        this._node.appendChild(titleNode);

        this._valueNode = createElement('p');
        this._valueNode.innerHTML = '-';
        this._node.appendChild(this._valueNode)
    }

    Card.prototype.appendTo = function(element){
        element.appendChild(this._node);
    }

    Card.prototype.update = function(data){
        var value = this._cardData.parse(data);
        if (value === null){
            this._valueNode.innerHTML = '-';
            return;
        }
        this._valueNode.innerHTML = this._cardData.format(value);
        if (this._cardData.shouldWarn && this._cardData.shouldWarn(value)){
            this._node.classList.add('warn');
        } else {
            this._node.classList.remove('warn');
        }
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
            header = createElement('h1'),
            progress = createElement('div'),
            cardContainer = createElement('div');

        header.innerHTML = document.title;
        body.appendChild(header);

        progress.classList.add('progress');
        this._progressValue = createElement('div');
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
        this._interval = 15;
        this._cardData = [
            {
                title: 'Lauke',
                parse: function(data){
                    if (data && 't7' in data){
                        return +data.t7 / 10;
                    }
                    return null;
                },
                format: function(value){
                    return value.toFixed(1) + deg;
                }
            },
            {
                title: 'Vanduo',
                parse: function(data){
                    if (data && 't3' in data && 't4' in data){
                        return { high: +data.t3 / 10, low: +data.t4 / 10 };
                    }
                    return null;
                },
                format: function(value){
                    return value.low.toFixed(1) + deg + ' ~ ' + value.high.toFixed(1) + deg;
                }
            },
            {
                title: 'Data',
                parse: function(data){
                    var parts;
                    if (data && 'date' in data){
                        parts = data.date.split(/\D/);
                        return { 
                            y: parts[0], 
                            m: parts[1], 
                            d: parts[2] 
                        };
                    }
                    return null;
                },
                format: function(value){
                    return value.y + '-' + value.m + '-' + value.d;
                }
            },
            {
                title: 'Laikas',
                parse: function(data){
                    var value;
                    if (data && 'time' in data){
                        value = +data.time % 1440; 
                        return {
                            h: ~~(value / 60), // value \ 60;
                            m: value % 60
                        };
                    }
                    return null;
                },
                format: function(value){
                    return ('0' + value.h).slice(-2) + ':' + ('0' + value.m).slice(-2); 
                }
            },
            {
                title: 'Šildymas',
                parse: function(data){
                    if (data && 't6' in data && 's11' in data){
                        return {
                            tflow: +data.t6 / 10,
                            tset: +data.s11 / 10                           
                        };
                    }
                    return null;
                },
                format: function(value){
                    return value.tflow.toFixed(1) + deg + ' / ' + value.tset.toFixed(1) + deg;
                } 
            },
            {
                title: 'Pranešimai',
                parse: function(data){
                    if (data && 'msg' in data){
                        return +data.msg;
                    }
                    return null;
                },
                format: function(value){
                    return value;
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
