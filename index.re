module React = ReasonReact;
module ReactDOM = ReactDOMRe;
let echo = React.stringToElement;

let handleRoute = (path, event) => {
  ReactEventRe.Mouse.preventDefault(event);
  React.Router.push(path)
};

module Button = {
  let component = React.statelessComponent("Button");
  let make = (~onClick, children) => {
    ...component,
    render: (_self) => <a className="button is-rounded" href="#" onClick>
      (React.arrayToElement(children))
    </a>
  }
};

module ReasonDemo = {
  type action = GotoIndex | GotoId(int) | Alert(string);

  [@bs.val] external alert: string => unit = "";
  let component = React.reducerComponent("ReasonDemo");
  let make = (_children) => {
    ...component,
    initialState: () => None,
    reducer: (action, _state) => switch (action) {
    | GotoIndex => React.Update(None)
    | GotoId(id) => React.Update(Some(id))
    | Alert(message) => React.SideEffects((_self) => {
      alert(message);
      React.Router.push("/index")
    })
  },
    subscriptions: (self) => [
      React.Sub(
        () => React.Router.watchUrl(url => switch (url.path) {
        | ["goto", id] => try (self.send(GotoId(int_of_string(id)))) {
          | Failure(_message) => ()
          }
        | ["index"] => self.send(GotoIndex)
        | _ => self.send(Alert("Danger, Will Robinson!"))
        }),

        React.Router.unwatchUrl
      )
    ],

    render: (self) => switch (self.state) {
    | None => <div>
        <h1>(echo("Index"))</h1>
        <Button onClick=handleRoute("/goto/1")>(echo("Goto 1"))</Button>
        <Button onClick=handleRoute("/goto/2")>(echo("Goto 2"))</Button>
        <Button onClick=handleRoute("/alert")>(echo("Alert"))</Button>
      </div>
    | Some(id) => <div>
        <h1>(echo("ID " ++ string_of_int(id)))</h1>
        <Button onClick=handleRoute("/goto/1")>(echo("Back to Index"))</Button>
      </div>
    }
  }
};

ReactDOM.renderToElementWithId(<ReasonDemo />, "index");