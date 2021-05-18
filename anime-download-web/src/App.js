import './App.css';
import 'bootstrap/dist/css/bootstrap.min.css';
import './sidebar.css'
//import 'bootstrap/dist/css/bootstrap.min.css';
import 'react-toastify/dist/ReactToastify.min.css';
import {
  HashRouter as Router,
} from "react-router-dom";
import AppRouter from './appRouter';
import { ToastContainer } from 'react-toastify';
import LinkApp from "./components/linkApp";
import "bootstrap-icons/font/bootstrap-icons.css";

function App() {
  return (
    <>
      <Router>
        <main>
          <>
            {/* <div className="d-flex flex-column flex-shrink-0 p-3 text-white bg-dark" style={{ "width": "280px", "height": "100vh" }}>
          <hr />
          <ul className="nav nav-pills flex-column mb-auto">
            <li className="nav-item">
              <a href="#" className="nav-link active" aria-current="page">
                <svg className="bi me-2" width="16" height="16">#home</svg>
          Home
        </a>
            </li>
            <li>
              <a href="#" className="nav-link text-white">
                <svg className="bi me-2" width="16" height="16">speedometer2</svg>
          Dashboard
        </a>
            </li>
            <li>
              <a href="#" className="nav-link text-white">
                <svg className="bi me-2" width="16" height="16">table</svg>
          Orders
        </a>
            </li>
            <li>
              <a href="#" className="nav-link text-white">
                <svg className="bi me-2" width="16" height="16"><a href="#grid" /></svg>
          Products
        </a>
            </li>
            <li>
              <a href="#" className="nav-link text-white">
                <svg className="bi me-2" width="16" height="16"><a href="#people-circle" /></svg>
          Customers
        </a>
            </li>
          </ul>
          <hr />
        </div>
        <LinkApp /> */}
          </>
          <LinkApp />
          <div className="container pt-5" style={{ "overflowY": "auto" }}>
            <ToastContainer autoClose={2000} />
            <div className="row">
              <div className="col-12">
                <AppRouter />
              </div>
            </div>
          </div>
        </main>
      </Router>
    </>
  );
}

export default App;
